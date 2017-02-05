#ifndef HITBOX_H
#define HITBOX_H
#include<SDL2/SDL.h>
#include "Entity.h"

class HitBox {
public:
	
	HitBox();
	~HitBox();
	
	bool collisionCheck(HitBox& nearHitBox);
	void move(float x, float y);
	void setRect(const SDL_Rect rect);
	const SDL_Rect& getRect();
	
	Entity* attached;
	
private:
	
	SDL_Rect rect;
	
};


#endif