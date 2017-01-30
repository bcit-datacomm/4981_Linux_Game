#include "Player.hpp"

Player::Player()
{
	playerSpriteClips[0].x = 0;
	playerSpriteClips[0].y = 0;
	playerSpriteClips[0].w = 100;
	playerSpriteClips[0].h = 100;
}

Player::~Player()
{
	this->playerTexture.free();	
}

void Player::handleInput(const Uint8 *state)
{
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

void Player::move(float moveX, float moveY)
{
	//Move the player left or right
	this->setX(this->getX()+moveX);

	//Move the player up or down
	this->setY(this->getY()+moveY);
	
}

void Player::setX(float px)
{
	x=px;
} //set x coordinate

void Player::setY(float py)
{
	y=py;
} //set y coordinate

void Player::setDX(float px)
{
	dx=px;
} //set delta x coordinate

void Player::setDY(float py)
{
	dy=py;
} //set delta y coordinate

void Player::setVelocity(int pvel)
{
	velocity = pvel;
} // set velocity of player movement

float Player::getX()
{
	return x;
} // get x coordinate

float Player::getY()
{
	return y;
} // get y coordinate

float Player::getDX()
{
	return dx;
} // get delta x coordinate

float Player::getDY()
{
	return dy;
} //get delta y coordinate

int Player::getVelocity()
{
	return velocity;
} // get velocity of player movement

std::string Player::getSpritePath()
{
	return spritePath;
}//returns path of players sprite
