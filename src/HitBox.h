#ifndef HITBOX_H
#define HITBOX_H
#include<SDL2/SDL.h>
#include "Entity.h"

class HitBox {
public:
	
	HitBox();
	~HitBox();
	
	void move(float x, float y);
	void setRect(const SDL_Rect rect);
	const SDL_Rect& getRect();
	
	void setFriendly(bool isFriendly);
	bool isPlayerFriendly();
	
	Entity* attached; // Entity that it is linked to
	
	
private:
	
	SDL_Rect rect;
	bool playerFriendly = false;
	
};


#endif