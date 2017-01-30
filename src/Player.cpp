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
