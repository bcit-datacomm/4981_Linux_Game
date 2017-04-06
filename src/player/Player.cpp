#include <math.h>

#include "Player.h"
#include "../log/EntityDump.h"
#include "../sprites/VisualEffect.h"
#include "../sprites/SpriteTypes.h"
#include "../game/GameHashMap.h"

Player::Player() : tempBarricadeID(-1), tempTurretID(-1), holdingTurret(false), pickupTick(0), pickupDelay(200),
        marine(nullptr) {}

void Player::setControl(Marine* newControl) {
    marine = newControl;
}

void Player::handleMouseUpdate(const int winWidth, const int winHeight, const float camX, const float camY) {
    int mouseX;
    int mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);
    const int mouseDeltaX = winWidth / 2 - mouseX;
    const int mouseDeltaY = winHeight / 2 - mouseY;

    marine->setAngle(((atan2(mouseDeltaX, mouseDeltaY)* 180.0)/M_PI) * - 1);

    if (tempBarricadeID > -1) {
        Barricade& tempBarricade = GameManager::instance()->getBarricade(tempBarricadeID);
        tempBarricade.move(marine->getX(), marine->getY(), mouseX + camX, mouseY + camY,
            GameManager::instance()->getCollisionHandler());
    }

    if (tempTurretID > -1) {
        Turret& tempTurret = GameManager::instance()->getTurret(tempTurretID);
        tempTurret.move(marine->getX(), marine->getY(), mouseX + camX, mouseY + camY,
            GameManager::instance()->getCollisionHandler());

        if (SDL_GetMouseState(nullptr, nullptr)  &SDL_BUTTON(SDL_BUTTON_RIGHT)) {
            if (tempTurret.collisionCheckTurret(marine->getX(), marine->getY(), mouseX + camX, mouseY + camY,
                    GameManager::instance()->getCollisionHandler())) {
                tempTurret.placeTurret();
                tempTurretID = -1;
                holdingTurret = false;
            }
        }
    }

    //fire weapon on left mouse click
    if (SDL_GetMouseState(nullptr, nullptr)  &SDL_BUTTON(SDL_BUTTON_LEFT)) {
        if (marine->inventory.getCurrent() != nullptr) {
            marine->fireWeapon();
        }
    }

}

void Player::handleMouseWheelInput(const SDL_Event *e) {
    marine->inventory.scrollCurrent(e->wheel.y);
}

// function to handle mouse-click events
void Player::handlePlacementClick(SDL_Renderer *renderer) {

    if (tempBarricadeID > -1) {
        Barricade& tempBarricade = GameManager::instance()->getBarricade(tempBarricadeID);
        if (tempBarricade.isPlaceable()) {
            tempBarricade.placeBarricade();
            tempBarricadeID = -1;
        }
    }
}

void Player::handleKeyboardInput(const int winWidth, const int winHeight, const Uint8 *state) {
    float x = 0;
    float y = 0;
    float velocity = marine->getVelocity();

    // Check for move inputs
    if (state[SDL_SCANCODE_UP] || state[SDL_SCANCODE_W]) {
        y -= velocity;
    }
    if (state[SDL_SCANCODE_DOWN] || state[SDL_SCANCODE_S]) {
        y += velocity;
    }
    if (state[SDL_SCANCODE_LEFT] || state[SDL_SCANCODE_A]) {
        x -= velocity;
    }
    if (state[SDL_SCANCODE_RIGHT] || state[SDL_SCANCODE_D]) {
        x += velocity;
    }

    //Inventory inputs
    if (state[SDL_SCANCODE_1]) {
        marine->inventory.switchCurrent(0);
    } else if (state[SDL_SCANCODE_2]) {
        marine->inventory.switchCurrent(1);
    } else if (state[SDL_SCANCODE_3]) {
        marine->inventory.switchCurrent(2);
    }

    //Weapon input
    if (state[SDL_SCANCODE_R]) {
        Weapon *w = marine->inventory.getCurrent();
        if (w) {
            w->reloadClip();
        }
    }
    //pickup button
    if (state[SDL_SCANCODE_E]) {
        const int currentTime = SDL_GetTicks();

        if (currentTime > (pickupTick + pickupDelay)) {
            pickupTick = currentTime;

            const int checkTurret = marine->checkForPickUp();
            if (checkTurret > -1 && holdingTurret == false)
            {
                tempTurretID = checkTurret;
                GameManager::instance()->getTurret(tempTurretID).pickUpTurret();
                holdingTurret = true;
            }
        }
    }
    //Drop button
    if (state[SDL_SCANCODE_F]) {
        marine->inventory.dropWeapon(marine->getX(), marine->getY());
    }
    //use Inventory
    if (state[SDL_SCANCODE_I]) {
        marine->inventory.useItem();
    }

    //added by Maitiu Debug print 5/3 / 2017
    if (state[SDL_SCANCODE_SPACE]) {
        //render guide arrows
        spawnArrowGuides(winWidth, winHeight);
    }

    //added by Maitiu Debug print 4/3 / 2017
    if (state[SDL_SCANCODE_PERIOD]) {
        dumpEntityPositions(this);
    }
    marine->setDY(y);
    marine->setDX(x);
}

void Player::handleTempBarricade(SDL_Renderer *renderer) {
    if (tempBarricadeID < 0) {
        if (!marine) {
            return;
        }
        const double angle = marine->getAngle();
        tempBarricadeID = GameManager::instance()->createBarricade(
            marine->getX() + PLAYER_PLACE_DISTANCE * cos(angle),
            marine->getY() + PLAYER_PLACE_DISTANCE * sin(angle));
    } else {
        GameManager::instance()->deleteBarricade(tempBarricadeID);
        tempBarricadeID = -1;
    }
}

void Player::handleTempTurret(SDL_Renderer *renderer) {
   if (tempTurretID < 0) {
       const double angle = marine->getAngle();
       tempTurretID = GameManager::instance()->createTurret(
           marine->getX() + PLAYER_PLACE_DISTANCE * cos(angle),
           marine->getY() + PLAYER_PLACE_DISTANCE * sin(angle));
   } else {
       GameManager::instance()->deleteTurret(tempTurretID);
       tempTurretID = -1;
   }
}

void Player::checkMarineState() {
    if (marine && marine->getHealth() <= 0) {
        GameManager::instance()->deleteMarine(marine->getId());
        setControl(nullptr);
    }
}

void Player::spawnArrowGuides(const int winWidth, const int winHeight) {
    VisualEffect &ve = VisualEffect::instance();
    GameManager *gm = GameManager::instance();
    auto &om = gm->getObjectManager();

    std::pair<float, float> bCoord = om[0].first.getDestCoord();
    double angle = getAngleBetweenPoints({marine->getX(), marine->getY()}, bCoord);

    std::pair<float, float> gCoord = getGuideCoord(angle, winWidth, winHeight);
    SDL_Rect baseGuide = {gCoord.first, gCoord.second, 100, 100};
    SDL_Rect baseSrc = {82, 44, 1012, 1050};
    ve.addPostTex(2, baseSrc, baseGuide, TEXTURES::BASE);

}

double Player::getAngleBetweenPoints(const std::pair<float, float> p1, const std::pair<float, float> p2) {
    return atan2(p1.second - p2.second, p1.first - p2.first);
}

std::pair<float, float> Player::getGuideCoord(const double radian, const int winWidth, const int winHeight) {
    double angle = 90 - (radian * 180/3.14159265);
    //Top of the screen so the base is north
    if (angle + 90 >= 40 && angle + 90 <= 145) {
        //going up
        double h = (tan(angle * 3.14159265/180) * ((double)winHeight / 2));
        double x;
        if (winWidth / 2 > abs(h)) {
            x = marine->getX() - h + 50;
        } else {
            //if ANgle Brings IMG out of screen set it to the screens width/2
            if (angle + 90 > 90) {
                x = marine->getX() - winWidth / 2;
            } else {
                x = marine->getX() + winWidth / 2;
            }
        }

        printf("X:%f\n", x);
        return{x, marine->getY() - (winHeight / 2)};

    } else if ((angle + 90 <= 323 && angle + 90 >= 216)) {//BASE IS DOWN
        double t = tan(angle * 3.14159265/180);
        double h = t * 480;
        double x;
        printf("DOWN\n");
        if (winWidth / 2 > abs(h)) {
            x = marine->getX() + h - 50;
        } else {

            if (angle + 90 > 270) {
                x = marine->getX() + winWidth / 2;
            } else {
                x = marine->getX() - winWidth / 2;
            }

        }

        printf("X:%f\n", x);
        return{x, marine->getY() + (winHeight / 2)};

    } else if (angle + 90 <= 40 || angle + 90 >= 323) {//BASE IS ON THE RIGHT
        double t = tan((angle + 90) * 3.14159265/180);
        double h = t * ((double)winWidth / 2);
        double y;
        if (winHeight / 2 > abs(h) - 100) {
            y = marine->getY() - h;
        } else {

            if (angle + 90 < 40) {
                y = marine->getY() - winHeight / 2;
            } else {
                y = marine->getY() + winHeight / 2;
            }

        }

        printf("Y:%f\n", y);
        return{marine->getX() + (winWidth / 2), y};

    } else if (angle + 90 <= 216 || angle + 90 >= 145) {//BASE IS ON THE LEFT
        double t = tan((angle + 90) * 3.14159265/180);
        double h = t * ((double)winWidth / 2);
        double y;
        if (winHeight / 2 > abs(h) - 100) {
            printf("Hit\n");
            y = marine->getY() + h;
        } else {

            if (angle + 90 < 40) {
                y = marine->getY() + winHeight / 2;
            } else {
                printf("Miss\n");
                y = marine->getY() - winHeight / 2;
            }

        }

        printf("Y:%f\n", y);
        return{marine->getX() - (winWidth / 2) + 100, y};
    }
    return {1, 2};
}
