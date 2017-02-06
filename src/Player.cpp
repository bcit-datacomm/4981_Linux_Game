#include "Player.h"

Player::Player() {

}

Player::~Player() {

}

void Player::setControl(Marine* newControl) {
	this->marine = newControl;
}

void Player::handleMouseInput(Window* w) {
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
