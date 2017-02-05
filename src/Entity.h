#ifndef ENTITY_H
#define ENTITY_H
#include <string>
#include "LTexture.h"
#include <SDL2/SDL.h>

class Entity {
public:	
	
	Entity();
	~Entity();
	
	virtual void onCollision() = 0;
	virtual void collidingProjectile(int damage) = 0;
	
	void setPosition(float x, float y); // Set marine position
	
	void setX(float px); //set x coordinate

    void setY(float py); //set y coordinate

	float getX(); // get x coordinate

    float getY(); // get y coordinate
	
	LTexture texture;
	
private:
	float x = 200; //x coordinate
    float y = 200; //y coordinate
};

#endif