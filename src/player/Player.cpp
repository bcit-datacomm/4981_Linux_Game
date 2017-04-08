#include <cmath>

#include "Player.h"
#include "../game/GameManager.h"
#include "../log/EntityDump.h"
#include "../sprites/VisualEffect.h"
#include "../sprites/SpriteTypes.h"
#include "../game/GameHashMap.h"
#include "../buildings/Base.h"

/**
* Date: Jan. 28, 2017
* Author: Jacob McPhail
* Modified: ---
* Function Interface: Player()
* Description:
*   ctor for a player.
*/
Player::Player() : tempBarricadeID(-1), tempTurretID(-1), holdingTurret(false),
        pickupTick(0), pickupDelay(200), respawnTick(0), marine(nullptr) {
    moveAction.id = static_cast<int32_t>(UDPHeaders::WALK);
    attackAction.id = static_cast<int32_t>(UDPHeaders::ATTACKACTIONH);
}

/**
* Date: Jan. 28, 2017
* Author: Jacob McPhail
* Modified: ---
* Function Interface: setControl(Marine* newControl)
*       newControl : Marine to control
*
* Description:
*   Set what marine the player controls.
*/
void Player::setControl(Marine* newControl) {
    marine = newControl;
}

/**------------------------------------------------------------------------------
Method: hasChangedAngle

Date: April 4, 2017

Designer: Brody McCrone

Programmer: Brody McCrone

Interface: bool hasChangedAngle()

Returns:
true: The players angle has changed since the last frame.
false: The players angle has NOT changed since the last frame.

Notes:
Checks if the players angle has changed since the last frame.
-------------------------------------------------------------------------------*/
bool Player::hasChangedAngle() const {
    return fabs(moveAction.data.ma.direction - marine->getAngle()) > DOUBLE_COMPARISON_PRECISION;
}

/**------------------------------------------------------------------------------
Method: hasChangedCourse

Date: April 4, 2017

Designer: Brody McCrone

Programmer: Brody McCrone

Interface: bool hasChangedCourse()

Returns:
true: The player's course has changed since the last frame.
false: The player's course has NOT changed since the last frame.

Notes:
Checks if the player's course has changed since the last frame.
-------------------------------------------------------------------------------*/
bool Player::hasChangedCourse() const {
    return moveAction.data.ma.xdel - marine->getDX()
            || moveAction.data.ma.ydel - marine->getDY();
}

/**------------------------------------------------------------------------------
Method: sendServMoveAction

Date: April 4, 2017

Designer: Brody McCrone

Programmer: Brody McCrone

Interface: bool sendServMoveAction

Returns:
void

Notes:
Updates the player's moveAction struct and send it to the server via UDP.
-------------------------------------------------------------------------------*/
void Player::sendServMoveAction() {
    moveAction.data.ma.id = id;
    moveAction.data.ma.xpos = marine->getX();
    moveAction.data.ma.ypos = marine->getY();
    moveAction.data.ma.xdel = marine->getDX();
    moveAction.data.ma.ydel = marine->getDY();
    moveAction.data.ma.vel = marine->getVelocity();
    moveAction.data.ma.direction = marine->getAngle();
    NetworkManager::instance().writeUDPSocket((char *)&moveAction, sizeof(ClientMessage));
}

/**------------------------------------------------------------------------------
Method: sendServAttackAction

Date: April 4, 2017

Designer: Brody McCrone

Programmer: Brody McCrone

Interface: bool sendServMoveAction

Returns:
void

Notes:
Updates the player's attack action and send it to the server via UDP.
-------------------------------------------------------------------------------*/
void Player::sendServAttackAction() {
    attackAction.data.aa.playerid = id;
    attackAction.data.aa.actionid = static_cast<int32_t>(UDPHeaders::SHOOT);
    attackAction.data.aa.weaponid = marine->inventory.getCurrent()->getID();
    attackAction.data.aa.xpos = marine->getX();
    attackAction.data.aa.ypos = marine->getY();
    attackAction.data.aa.direction = marine->getAngle();

    NetworkManager::instance().writeUDPSocket((char *)&attackAction, sizeof(ClientMessage));
}

/**
* Date: Feb. 6, 2017
* Author: Jacob McPhail
* Modified: ---
* Function Interface: handleMouseUpdate(const int winWidth, const int winHeight,
*                const float camX, const float camY)
*       winWidth : Window width
*       winHeight : Window height
*       camX : Camera x position
*       camY : Camera y position
*
* Description:
*   Handle user mouse input.
*
* Revisions:
* Apr. 04, 2017, Mark Chen - Adjusted the turret placements to properly handle mouse clicks
* Apr. 07, 2017, Mark Chen - Can no longer shoot while holding a turret.
*/
void Player::handleMouseUpdate(const int winWidth, const int winHeight, const float camX, const float camY) {
    int mouseX;
    int mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);
    const int mouseDeltaX = winWidth / 2 - mouseX;
    const int mouseDeltaY = winHeight / 2 - mouseY;

    marine->setAngle(((atan2(mouseDeltaX, mouseDeltaY)* ONE_EIGHTY)/M_PI) * - 1);

    if (tempBarricadeID > -1) {
        Barricade& tempBarricade = GameManager::instance()->getBarricade(tempBarricadeID);
        tempBarricade.move(marine->getX(), marine->getY(), mouseX + camX, mouseY + camY,
            GameManager::instance()->getCollisionHandler());
    }

    if (tempTurretID > -1) {
        Turret& tempTurret = GameManager::instance()->getTurret(tempTurretID);
        tempTurret.move(marine->getX(), marine->getY(), mouseX + camX, mouseY + camY,
            GameManager::instance()->getCollisionHandler());

        if (SDL_GetMouseState(nullptr, nullptr) & SDL_BUTTON(SDL_BUTTON_LEFT)) {
            if (tempTurret.collisionCheckTurret(marine->getX(), marine->getY(), mouseX + camX, mouseY + camY,
                    GameManager::instance()->getCollisionHandler())) {
                tempTurret.placeTurret();
                tempTurretID = -1;
                holdingTurret = false;
            }
        }
    } else if (SDL_GetMouseState(nullptr, nullptr)  &SDL_BUTTON(SDL_BUTTON_LEFT)) {
            if(marine->inventory.getCurrent()) {
                marine->fireWeapon();
            }
        }
}
/*
    //fire weapon on left mouse click
    if (SDL_GetMouseState(nullptr, nullptr)  &SDL_BUTTON(SDL_BUTTON_LEFT)) {
        if(marine->inventory.getCurrent()) {
            marine->fireWeapon();
        }
    }*/

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


/**
* Date: Feb. 6, 2017
* Author: Jacob McPhail
* Modified: ---
* Function Interface: handleKeyboardInput(const Uint8 *state)
*         state : Keyboard state
*
* Description:
*   Handle user key input.
*/
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
        spawnMapGuides(winWidth, winHeight);
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
        if (!marine) {
            return;
        }
        const double angle = marine->getAngle();
        tempTurretID = GameManager::instance()->createTurret(
            marine->getX() + PLAYER_PLACE_DISTANCE * cos(angle),
            marine->getY() + PLAYER_PLACE_DISTANCE * sin(angle));
    } else {
        GameManager::instance()->deleteTurret(tempTurretID);
        tempTurretID = -1;
    }
}


/**
 * Date: Apl. 5, 2017
 * Author: Jacob McPhail
 * Function Interface: checkMarineState()
 * Description:
 *      Check marine health if 0 kill marine, also check if player respawns.
 */
bool Player::checkMarineState() {
    if (marine && marine->getHealth() <= 0) {
        GameManager::instance()->deleteMarine(marine->getId());
        respawnTick = SDL_GetTicks();
        setControl(nullptr);
        return false;
    }
    return !marine && (static_cast<int>(SDL_GetTicks()) >= (respawnTick + RESPAWN_DELAY));
}

/**
 * Date: Apl. 5, 2017
 * Author: Jacob McPhail
 * Function Interface: respawn(Point newPoint)
 *      newPoint : Player respawn point
 *
 * Description:
 *      Respawn player with a new marine.
 */
void Player::respawn(const Point& newPoint) {
    const int32_t playerMarineID = GameManager::instance()->createMarine();
    //gives the player control of the marine
    setControl(&GameManager::instance()->getMarine(playerMarineID).first);
    getMarine()->setPosition(newPoint.first, newPoint.second);
    getMarine()->setSrcRect(SPRITE_FRONT, SPRITE_FRONT, SPRITE_SIZE_X, SPRITE_SIZE_Y);
}

/**
 * Date: Apl. 5, 2017
 * Author: Maitiu Morton
 * Function Interface: spawnMapGuides(const int winWidth, const int winHeight)
 *      winWidth: windows width
 *      winHeight: windows height
 * Description:
 *      Gets the base entity and store entities and renders their guides on the edge of the screen by
 *      using getAngle and getGuideCoord functions.
 */
void Player::spawnMapGuides(const int winWidth, const int winHeight) {
    VisualEffect &ve = VisualEffect::instance();
    GameManager *gm = GameManager::instance();

    //BASE
    Base base = gm->getBase();
    std::pair<float, float> bCoord = base.getDestCoord();
    double angle = getAngleBetweenPoints({marine->getX(), marine->getY()}, bCoord);

    const std::pair<float, float> gCoord = getGuideCoord(angle, winWidth, winHeight);
    //Rect for BASE guide img
    SDL_Rect baseGuide = {static_cast<int>(gCoord.first), static_cast<int>(gCoord.second), GUIDE_SIZE, GUIDE_SIZE};
    ve.addPostTex(2, base.getSrcRect(), baseGuide, TEXTURES::BASE);


    //STORES
    for (const auto& s : gm->getStoreManager()) {
        const std::pair<float, float> destCoord = getDestCoordinates(s.second.get());
        double angle = getAngleBetweenPoints({marine->getX(), marine->getY()}, destCoord);
        const std::pair<float, float> gCoord = getGuideCoord(angle, winWidth, winHeight);
        //RECT for Store Guide img
        SDL_Rect StoreGuide = {static_cast<int>(gCoord.first), static_cast<int>(gCoord.second),
                GUIDE_SIZE / 2, GUIDE_SIZE};
        ve.addPostTex(2, s.second->getSrcRect(), StoreGuide, TEXTURES::MAP_OBJECTS);
    }

    //DROP ZONE


}

/**
 * Date: Apl. 5, 2017
 * Author: Maitiu Morton
 * Function Interface:getAngleBetweenPoints(const std::pair<float, float> p1, const std::pair<float, float> p2)
 *      p1: corrdinates of the first point
 *      p2: corrdinates of the second point
 *
 * Description:
 *      GEts the angle between two points in the world.
 */
double Player::getAngleBetweenPoints(const std::pair<float, float> p1, const std::pair<float, float> p2) {
    return atan2(p1.second - p2.second, p1.first - p2.first);
}

/**
 * Date: Apl. 5, 2017
 * Author: JMaitiu Morton
 * Function Interface: getGuideCoord(const double radian, const int winWidth, const int winHeight)
 *      radian : degree of object from player in Radians
 *      winWidth: window width
 *      winHeight: window height
 *
 * Description:
 *      comverts radian to an angle and then compares it with the angles of the four corners so that
 *      it can call the appropriate calulate coordinates function.
 */
std::pair<float, float> Player::getGuideCoord(const double radian, const int winWidth, const int winHeight) {

    double angle = NINTY_DEGREES - (radian * ONE_EIGHTY/M_PI);

    if (angle + NINTY_DEGREES >= TOP_RIGHT_ANGLE && angle + NINTY_DEGREES <= TOP_LEFT_ANGLE) {
        //ENTITY IS NORTH
        return calculateHorizontalCoords(angle, NINTY_DEGREES, winWidth, winHeight);

    } else if ((angle + NINTY_DEGREES <= BOTTOM_RIGHT_ANGLE && angle + NINTY_DEGREES >= BOTTOM_LEFT_ANGLE)) {
        //BASE IS SOUTH
        return calculateHorizontalCoords(angle, TWO_SEVENTY_DEGREES, winWidth, winHeight);

    } else if (angle + NINTY_DEGREES <= TOP_LEFT_ANGLE || angle + NINTY_DEGREES >= BOTTOM_RIGHT_ANGLE) {
        //BASE IS ON THE RIGHT
        return calculateVerticleCoords(angle + NINTY_DEGREES, TOP_LEFT_ANGLE, winWidth, winHeight);

    } else {
        //BASE IS ON THE LEFT
        return calculateVerticleCoords(angle + NINTY_DEGREES, ONE_SIXTY_DEGREES, winWidth, winHeight);
    }
}

/**
 * Date: Apl. 5, 2017
 * Author: Maitiu Morton
 * Function Interface: calculateVerticleCoords(const double angle, const int compareDegree, const int winWidth,
 *                                             const int winHeight)
 *      angle : angle of the object compared to the player in degrees
 *      compareDegree: a dregree the angle will be compared to depending on which direction the Entity is in
 *      winWidth: window width
 *      winHeight: window height
 *
 * Description:
 *      THis calcualte coordinates function is for when the Entity is above or below the player. It uses the angle
 *      between the player along with the widht and height of the screen to calculates the coordinates for the guide
 */
std::pair<float, float> Player::calculateVerticleCoords(const double angle, const int compareDegree, const int winWidth, const int winHeight) {

    double t = tan((angle) * M_PI/ONE_EIGHTY);
    double h = t * (winWidth / 2);//the difference on the x axis compared to the player
    double y;
    if (winHeight / 2 > abs(h) + VERTICAL_ADJUST) {//if not going off screen
        if(compareDegree == TOP_LEFT_ANGLE) {//if on right
            y = marine->getY() - h - GUIDE_SIZE;
        } else {
            y = marine->getY() + h - GUIDE_SIZE;//on left
        }
    } else {
        if (angle < compareDegree) {
            y = marine->getY() - winHeight / 2 + TOP_AND_LEFT_PADDING;//go to top corner
        } else {
            y = marine->getY() + winHeight / 2 - BOTTOM_PADDING;//go to bottom corner
        }

    }
    if(compareDegree == TOP_LEFT_ANGLE) {//if on right
        return {marine->getX() + (winWidth / 2) - TOP_AND_LEFT_PADDING, y};
    }
    return {marine->getX() - (winWidth / 2) + MAX_WIDTH, y};//on left
}

/**
 * Date: Apl. 5, 2017
 * Author: Maitiu Morton
 * Function Interface: calculateHorizontalCoords(const double angle, const int compareDegree, const int winWidth,
 *                                               const int winHeight)
 *      angle : angle of the object compared to the player in degrees
 *      compareDegree: a dregree the angle will be compared to depending on which direction the Entity is in
 *      winWidth: window width
 *      winHeight: window height
 *
 * Description:
 *      THis calcualte coordinates function is for when the Entity is Left or right of the player. It uses the angle
 *      between the player along with the width and height of the screen to calculates the coordinates for the guide
 */
std::pair<float, float> Player::calculateHorizontalCoords(const double angle, const int compareDegree, const int winWidth, const int winHeight) {

    double h = (tan(angle * M_PI/ONE_EIGHTY) * (winHeight / 2));//the difference on the y axis compared to the player
    double x;

    if ((winWidth / 2) + MAX_WIDTH > abs(h) + HORIZONTAL_ADJUST) {
        if(compareDegree == NINTY_DEGREES) {//is top or bottom
            x = marine->getX() - h + TOP_AND_LEFT_PADDING;
        } else {
            x = marine->getX() + h - RIGHT_PADDING;
        }
    } else {

        //if ANgle Brings IMG out of screen set it to the screens width/2
        if (angle + NINTY_DEGREES > compareDegree) {
            switch(compareDegree) {//is top or bottom
                case(NINTY_DEGREES):
                    x = marine->getX() - winWidth / 2 + RIGHT_PADDING;
                    break;
                case(TWO_SEVENTY_DEGREES):
                    x = marine->getX() + winWidth / 2 - TOP_AND_LEFT_PADDING;
                    break;
            }
        } else {
            switch(compareDegree) {//is top or bottom
                case(NINTY_DEGREES):
                    x = marine->getX() + winWidth / 2 - TOP_AND_LEFT_PADDING;
                    break;
                case(TWO_SEVENTY_DEGREES):
                    x = marine->getX() - winWidth / 2 + RIGHT_PADDING;
                    break;
            }
        }

    }
    if(compareDegree == NINTY_DEGREES) {
        return {x, marine->getY() - (winHeight / 2) + TOP_AND_LEFT_PADDING};
    }
    return {x, marine->getY() + (winHeight / 2) - BOTTOM_PADDING};
}
