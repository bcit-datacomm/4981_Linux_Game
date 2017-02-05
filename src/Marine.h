#ifndef MARINE_H
#define MARINE_H
#include <string>
#include "LTexture.h"
#include "HitBox.h"
#include "Entity.h"
#include "CollisionHandler.h"
#include <vector>
#include <SDL2/SDL.h>

const int MARINE_HEIGHT = 100;
const int MARINE_WIDTH = 100;

class Marine : public Entity {
public:
    void create(); //function displays character

	void move(float moveX, float moveY, CollisionHandler* ch); // Moves Marine
	
    void setDX(float px); //set delta x coordinate

    void setDY(float py); //set delta y coordinate

    void setVelocity(int pvel); // set velocity of Marine movement

    float getDX(); // get delta x coordinate

    float getDY(); //get delta y coordinate

    int	getVelocity(); // get velocity of Marine movement
	
	void onCollision();
	
	void collidingProjectile(int damage);

	void setPosition(float x, float y); // Set marine position
	
	void setX(float px); //set x coordinate

    void setY(float py); //set y coordinate
	
	Marine();
	virtual ~Marine();
	
	SDL_Rect marineSpriteClips[1];
	HitBox movementHitBox;
	HitBox projectileHitBox;
	
private:
    float dx; // delta x coordinat
    float dy; //delta ycoordinate
    int velocity = 500; // velocity of Marine movement
    int health;
    int state; //used to select sprite to display

};

#endif
