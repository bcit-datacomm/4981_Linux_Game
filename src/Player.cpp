#include "Player.h"
#include <math.h>

Player::Player() : tempBarricadeID(-1) {

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
        tempBarricade.move(marine->getX(), marine->getY(), mouseX + camX, mouseY + camY, GameManager::instance()->getCollisionHandler());
    }

}

void Player::handleMouseWheelInput(const SDL_Event *e){
    marine->inventory.scrollCurrent(e->wheel.y);
}

// function to handle mouse-click events
void Player::handlePlacementClick(SDL_Renderer *renderer) {
    double angle;
    float marineX = marine->getX();
    float marineY = marine->getY();
    // getting mouses current position
    angle = marine->getAngle();

    if (tempBarricadeID > -1) {
        Barricade &tempBarricade = GameManager::instance()->getBarricade(tempBarricadeID);
	    if (tempBarricade.isPlaceable()) {
		    tempBarricade.placeBarricade();
            tempBarricadeID = -1;
	    }
	    return;
    }

    unsigned int tid = GameManager::instance()->createTurret();

    Turret &dumbTurret = GameManager::instance()->getTurret(tid);
    if (!dumbTurret.texture.loadFromFile("assets/texture/turret.png",
                                                                    renderer)) {
        printf("Failed to load the player texture!\n");
    }

    // calculates which spot to place turret based on player mouse direction
    if (angle <= 22.5 && angle >= -22.5) {
       turretPlaceCheck(marineX, marineY-100, GameManager::instance()->getCollisionHandler(), dumbTurret, tid);
    } else if (angle > 22.5 && angle <= 67.5) {
       turretPlaceCheck(marineX + 100, marineY - 100, GameManager::instance()->getCollisionHandler()
                        , dumbTurret, tid);
    } else if (angle > 67.5 && angle <= 112.5) {
       turretPlaceCheck(marineX + 100, marineY, GameManager::instance()->getCollisionHandler()
                        , dumbTurret, tid);
    } else if (angle > 112.5 && angle <= 157.5) {
       turretPlaceCheck(marineX + 100, marineY + 100, GameManager::instance()->getCollisionHandler()
                        , dumbTurret, tid);
    } else if (angle < -157.5 || angle > 157.5) {
       turretPlaceCheck(marineX, marineY+100, GameManager::instance()->getCollisionHandler()
                        , dumbTurret, tid);
    } else if (angle >= -67.5 && angle < -22.5) {
       turretPlaceCheck(marineX - 100, marineY - 100, GameManager::instance()->getCollisionHandler()
                        , dumbTurret, tid);
    } else if (angle >= -112.5 && angle <= -67.5) {
       turretPlaceCheck(marineX-100, marineY, GameManager::instance()->getCollisionHandler()
                        , dumbTurret, tid);
    } else if (angle >= -157.5 && angle <= -112.5) {
       turretPlaceCheck(marineX - 100, marineY + 100, GameManager::instance()->getCollisionHandler()
                        , dumbTurret, tid);
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
        marine->inventory.getCurrent().reloadClip();
    }
    if(state[SDL_SCANCODE_E]){
        marine->inventory.pickUp();
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
    }
    else {
        GameManager::instance()->deleteBarricade(tempBarricadeID);
        tempBarricadeID = -1;
    }
}



// checks for collision and to whether or not to place the turret
void Player::turretPlaceCheck(float x, float y, CollisionHandler& collisionHandler, Turret& dumbTurret,
                      unsigned int tid){
   if(dumbTurret.collisionCheckTurret(x, y, collisionHandler)){
          dumbTurret.setPosition(x, y);
   } else {
        printf("\nCANNOT PLACE TURRET HERE\n");
        GameManager::instance()->deleteTurret(tid);
    }
}
