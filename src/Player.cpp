#include "Player.h"
#include <math.h>

Player::Player() {

}

Player::~Player() {

}

void Player::setControl(Marine* newControl) {
	this->marine = newControl;
}

void Player::handleMouseUpdate(Window* w, float camX, float camY) {
	int mouseX;
    int mouseY;
    int mouseDeltaX;
    int mouseDeltaY;
    double radianConvert = 180.0000;

    SDL_GetMouseState(&mouseX, &mouseY);
    mouseDeltaX = w->getWidth()/2 - mouseX;
    mouseDeltaY = w->getHeight()/2 - mouseY;

    double angle = ((atan2(mouseDeltaX, mouseDeltaY)* radianConvert)/M_PI) * - 1;
	this->marine->setAngle(angle);

    if (this->tempBarricade != NULL) {
	this->tempBarricade->move(this->marine->getX(), this->marine->getY(), mouseX+camX, mouseY+camY, GameManager::instance()->getCollisionHandler());
    }
}

void Player::handleMouseWheelInput(const SDL_Event *e){
	this->marine->inventory.scrollCurrent(e->wheel.y);
}

// function to handle mouse-click events
void Player::handlePlacementClick(SDL_Renderer *renderer) {
    double angle;
    float marineX = this->marine->getX();
    float marineY = this->marine->getY();
    // getting mouses current position
	angle = this->marine->getAngle();

    if (this->tempBarricade != NULL) {
	if (this->tempBarricade->isPlaceable()) {
		GameManager::instance()->placeTempBarricade();
		this->tempBarricade = NULL;
	}
	return;
    }

    unsigned int tid = GameManager::instance()->createTurret();

	Turret* dumbTurret = GameManager::instance()->getTurret(tid);
	if (!dumbTurret->texture.loadFromFile("assets/texture/turret.png",
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
	float velocity = this->marine->getVelocity();

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
		this->marine->inventory.switchCurrent(0);
	} else if (state[SDL_SCANCODE_2]){
		this->marine->inventory.switchCurrent(1);
	} else if (state[SDL_SCANCODE_3]){
		this->marine->inventory.switchCurrent(2);
	}
	this->marine->setDY(y);
	this->marine->setDX(x);
}

void Player::placeBarricade(SDL_Renderer *renderer) {
	if(this->tempBarricade == NULL){
		double angle = this->marine->getAngle();
		int distance = 100;
		GameManager::instance()->createTempBarricade(renderer, this->marine->getX() + distance*cos(angle), 
			this->marine->getY() + distance*sin(angle));
		this->tempBarricade = GameManager::instance()->getTempBarricade();
	}
	else{
		GameManager::instance()->deleteTempBarricade();
		this->tempBarricade = NULL;
	}
}



// checks for collision and to whether or not to place the turret
void Player::turretPlaceCheck(float x, float y, CollisionHandler* collisionHandler, Turret* dumbTurret,
                      unsigned int tid){
   if(dumbTurret->collisionCheckTurret(x, y, collisionHandler)){
   	   dumbTurret->setPosition(x, y);
   } else {
        printf("\nCANNOT PLACE TURRET HERE\n");
        GameManager::instance()->deleteTurret(tid);
    }
}
