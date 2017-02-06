#include "Player.h"

Player::Player() {

}

Player::~Player() {

}

void Player::setControl(Marine* newControl) {
	this->marine = newControl;
}

void Player::handleInput(const Uint8 *state) {
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

	this->marine->setDY(y);
	this->marine->setDX(x);
}
