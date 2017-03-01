#ifndef OBJECT_H
#define OBJECT_H
#include <string>
#include "LTexture.h"
#include <SDL2/SDL.h>
#include "HitBox.h"
class Object : public Entity {
public:	
	
	Object();
	virtual ~Object();

	void setPosition(float, float);
	void setX(float);
	void setY(float);
	void setHeight(int);
	void setWidth(int);
	int getHeight();
	int getWidth();
	
	SDL_Rect objectSpriteClips[1];
	HitBox movementHitBox;	// Hit box for movement
	HitBox projectileHitBox; // Hit box for projectile and bullets
	HitBox damageHitBox;
private:
	int height; // Height of object
	int width; // Width of object
};

#endif
