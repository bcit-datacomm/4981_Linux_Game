#include <math.h>

#include "Player.h"
#include "../log/EntityDump.h"

Player::Player() : tempBarricadeID(-1), tempTurretID(-1), holdingTurret(false), pickupTick(0), pickupDelay(200),
        marine(nullptr) {}

void Player::setControl(Marine& newControl) {
    marine = &newControl;
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
        if(marine->inventory.getCurrent() != nullptr){
            marine->fireWeapon();
        }
    }

}

void Player::handleMouseWheelInput(const SDL_Event *e){
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

void Player::handleKeyboardInput(const Uint8 *state) {
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
    if (state[SDL_SCANCODE_1]){
        marine->inventory.switchCurrent(0);
    } else if (state[SDL_SCANCODE_2]){
        marine->inventory.switchCurrent(1);
    } else if (state[SDL_SCANCODE_3]){
        marine->inventory.switchCurrent(2);
    }

    //Weapon input
    if(state[SDL_SCANCODE_R]){
        Weapon *w = marine->inventory.getCurrent();
        if(w){
            w->reloadClip();
        }
    }
    //pickup button
    if(state[SDL_SCANCODE_E]){
        const int currentTime = SDL_GetTicks();

        if(currentTime > (pickupTick + pickupDelay)) {
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
    if(state[SDL_SCANCODE_F]){
        marine->inventory.dropWeapon(marine->getX(), marine->getY());
    }
    //use Inventory
    if(state[SDL_SCANCODE_I]) {
        marine->inventory.useItem();
    }

    //added by Maitiu Debug print 4/3/2017
    if(state[SDL_SCANCODE_PERIOD]){
        dumpEntityPositions(this);
    }
    marine->setDY(y);
    marine->setDX(x);
}

void Player::handleTempBarricade(SDL_Renderer *renderer) {
    if(tempBarricadeID < 0) {
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
   if(tempTurretID < 0) {
       const double angle = marine->getAngle();
       tempTurretID = GameManager::instance()->createTurret(
           marine->getX() + PLAYER_PLACE_DISTANCE * cos(angle),
           marine->getY() + PLAYER_PLACE_DISTANCE * sin(angle));
   } else {
       GameManager::instance()->deleteTurret(tempTurretID);
       tempTurretID = -1;
   }
}
