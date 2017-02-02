#include "Player.h"

Player::Player() {
	playerSpriteClips[0].x = 0;
	playerSpriteClips[0].y = 0;
	playerSpriteClips[0].w = 100;
	playerSpriteClips[0].h = 100;
}

Player::~Player() {
	this->playerTexture.free();	
}

void Player::handleInput(const Uint8 *state) {
	float x = 0;
	float y = 0;
	float velocity = this->getVelocity();
	
	// Check for move inputs
	if (state[SDL_SCANCODE_UP] || state[SDL_SCANCODE_W]) 
	{
		y -= velocity;
	}
	if (state[SDL_SCANCODE_DOWN] || state[SDL_SCANCODE_S]) 
	{
		y += velocity;
	}
	if (state[SDL_SCANCODE_LEFT] || state[SDL_SCANCODE_A]) 
	{
		x -= velocity;
	}
	if (state[SDL_SCANCODE_RIGHT] || state[SDL_SCANCODE_D]) 
	{
		x += velocity;
	}

	this->setDY(y);
	this->setDX(x);
}

// Move player by x and y amount
void Player::move(float moveX, float moveY) {
	//Move the player left or right
	this->setX(this->getX()+moveX);

	//Move the player up or down
	this->setY(this->getY()+moveY);
	
}

// Set x coordinate
void Player::setX(float px) {
	x = px;
} 

// Set y coordinate
void Player::setY(float py) {
	y = py;
} 

// Set delta x coordinate
void Player::setDX(float px) {
	dx = px;
} 

// Set delta y coordinate
void Player::setDY(float py) {
	dy = py;
} 

// set velocity of player movement
void Player::setVelocity(int pvel) {
	velocity = pvel;
} 

// Get x coordinate
float Player::getX() {
	return x;
} 

// Get y coordinate
float Player::getY() {
	return y;
} 

// Get delta x coordinate
float Player::getDX() {
	return dx;
} 

// Get delta y coordinate
float Player::getDY() {
	return dy;
} 

// Get velocity of player movement
int Player::getVelocity() {
	return velocity;
} 

