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
