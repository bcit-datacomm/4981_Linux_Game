#include "Player.h"

Player::Player() {

	//front
	playerSpriteClips[ 0 ].x = 0;
	playerSpriteClips[ 0 ].y = 0;
	playerSpriteClips[ 0 ].w = PLAYER_WIDTH;
	playerSpriteClips[ 0 ].h = PLAYER_HEIGHT;

	//right 45 degree
	playerSpriteClips[ 1 ].x = 75;
	playerSpriteClips[ 1 ].y = 0;
	playerSpriteClips[ 1 ].w = PLAYER_WIDTH;
	playerSpriteClips[ 1 ].h = PLAYER_HEIGHT;

	//left 45 degree
	playerSpriteClips[ 2 ].x = 130;
	playerSpriteClips[ 2 ].y = 0;
	playerSpriteClips[ 2 ].w = PLAYER_WIDTH;
	playerSpriteClips[ 2 ].h = PLAYER_HEIGHT;

	//left 90 degree
	playerSpriteClips[ 3 ].x = 205;
	playerSpriteClips[ 3 ].y = 0;
	playerSpriteClips[ 3 ].w = PLAYER_WIDTH;
	playerSpriteClips[ 3 ].h = PLAYER_HEIGHT;

	//left 90 degree
	playerSpriteClips[ 4 ].x = 280;
	playerSpriteClips[ 4 ].y = 0;
	playerSpriteClips[ 4 ].w = PLAYER_WIDTH;
	playerSpriteClips[ 4 ].h = PLAYER_HEIGHT;

	//right 135 degree
	playerSpriteClips[ 5 ].x = 355;
	playerSpriteClips[ 5 ].y = 0;
	playerSpriteClips[ 5 ].w = PLAYER_WIDTH;
	playerSpriteClips[ 5 ].h = PLAYER_HEIGHT;

	//left 135 degree
	playerSpriteClips[ 6 ].x = 430;
	playerSpriteClips[ 6 ].y = 0;
	playerSpriteClips[ 6 ].w = PLAYER_WIDTH;
	playerSpriteClips[ 6 ].h = PLAYER_HEIGHT;

	//back
	playerSpriteClips[ 7 ].x = 505;
	playerSpriteClips[ 7 ].y = 0;
	playerSpriteClips[ 7 ].w = PLAYER_WIDTH;
	playerSpriteClips[ 7 ].h = PLAYER_HEIGHT;
}

Player::~Player() {
	this->playerTexture.free();	
}

void Player::handleInput(const Uint8 *state, SDL_Renderer* renderer) {
	float x = 0;
	float y = 0;
	float velocity = this->getVelocity();
	int direction = 0;
	
	// Check for move inputs
	if (state[SDL_SCANCODE_UP] || state[SDL_SCANCODE_W]) 
	{
		y -= velocity;
		direction = 7;
		
		if (!playerTexture.loadFromFile("assets/texture/5mohawk_back.png", renderer)) {
			printf("Failed to load the player texture!\n");
		}
	}
	if (state[SDL_SCANCODE_DOWN] || state[SDL_SCANCODE_S]) 
	{
		y += velocity;
		direction = 0;

		if (!playerTexture.loadFromFile("assets/texture/1mohawk_front.png", renderer)) {
			printf("Failed to load the player texture!\n");
		}
	}
	if (state[SDL_SCANCODE_LEFT] || state[SDL_SCANCODE_A]) 
	{
		x -= velocity;
		direction = 4;

		if (!playerTexture.loadFromFile("assets/texture/3mohawk-90.png", renderer)) {
			printf("Failed to load the player texture!\n");
		}
	}
	if (state[SDL_SCANCODE_RIGHT] || state[SDL_SCANCODE_D]) 
	{
		x += velocity;
		direction = 5;

		if (!playerTexture.loadFromFile("assets/texture/3mohawk+90.png", renderer)) {
			printf("Failed to load the player texture!\n");
		}
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


