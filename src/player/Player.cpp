#include "Player.h"
#include <math.h>

Player::Player() : tempBarricadeID(-1), tempTurretID(-1) {

}

Player::~Player() {

}

void Player::setControl(Marine& newControl) {
    marine = &newControl;
}


void Player::handleMouseUpdate(Window& w, float camX, float camY) {
    int mouseX;
    int mouseY;
    int mouseDeltaX;
    int mouseDeltaY;
    double radianConvert = 180.0000;

    SDL_GetMouseState(&mouseX, &mouseY);
    mouseDeltaX = w.getWidth()/2 - mouseX;
    mouseDeltaY = w.getHeight()/2 - mouseY;

    double angle = ((atan2(mouseDeltaX, mouseDeltaY)* radianConvert)/M_PI) * - 1;

    marine->setAngle(angle);

    if (tempBarricadeID > -1) {
        Barricade &tempBarricade = GameManager::instance()->getBarricade(tempBarricadeID);
        tempBarricade.move(marine->getX(), marine->getY(), mouseX + camX, mouseY + camY,
            GameManager::instance()->getCollisionHandler());
    }

    if (tempTurretID > -1) {
        Turret &tempTurret = GameManager::instance()->getTurret(tempTurretID);
        tempTurret.move(marine->getX(), marine->getY(), mouseX + camX, mouseY + camY,
            GameManager::instance()->getCollisionHandler());

        if (SDL_GetMouseState(nullptr, nullptr) & SDL_BUTTON(SDL_BUTTON_RIGHT)) {
            if (tempTurret.collisionCheckTurret(marine->getX(), marine->getY(), mouseX + camX, mouseY + camY,
                    GameManager::instance()->getCollisionHandler())) {
                tempTurret.placeTurret();
                tempTurretID = -1;
            }
        }
    }

    //fire weapon on left mouse click
    if (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_LEFT)) {
        if(marine->inventory.getCurrent() != nullptr){
            if(marine->inventory.getCurrent()->getFireState()){
                marine->fireWeapon();
            }
        }
    }

}

void Player::handleMouseWheelInput(const SDL_Event *e){
    marine->inventory.scrollCurrent(e->wheel.y);
}

// function to handle mouse-click events
void Player::handlePlacementClick(SDL_Renderer *renderer) {

    if (tempBarricadeID > -1) {
        Barricade &tempBarricade = GameManager::instance()->getBarricade(tempBarricadeID);
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
        marine->inventory.getCurrent()->reloadClip();
    }
    if(state[SDL_SCANCODE_E]){
        marine->checkForPickUp();
    }
    if(state[SDL_SCANCODE_I]) {
        marine->inventory.useItem();
    }
    marine->setDY(y);
    marine->setDX(x);
}

void Player::handleTempBarricade(SDL_Renderer *renderer) {
    if(tempBarricadeID < 0) {
        double angle = marine->getAngle();
        int distance = 100;
        tempBarricadeID = GameManager::instance()->createBarricade(renderer, marine->getX() + distance*cos(angle),

            marine->getY() + distance*sin(angle));
    } else {
        GameManager::instance()->deleteBarricade(tempBarricadeID);
        tempBarricadeID = -1;
    }
}


void Player::handleTempTurret(SDL_Renderer *renderer) {
   if(tempTurretID < 0) {
       double angle = marine->getAngle();
       int distance = 100;
       tempTurretID = GameManager::instance()->createTurret(renderer, marine->getX() + distance*cos(angle),
           marine->getY() + distance*sin(angle));
   } else {
       GameManager::instance()->deleteTurret(tempTurretID);
       tempTurretID = -1;
   }
}
