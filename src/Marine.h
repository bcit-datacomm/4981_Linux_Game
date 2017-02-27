#ifndef MARINE_H
#define MARINE_H
#include <string>
#include "LTexture.h"
#include "HitBox.h"
#include "Entity.h"
#include "CollisionHandler.h"
#include "Inventory.h"
#include <math.h>
#include <vector>
#include <SDL2/SDL.h>
#include "Window.h"

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

    void setAngle(double a);//sets angle of sprite to

    double getAngle(); //returns sprites angle

	Marine();
	virtual ~Marine();

	SDL_Rect marineSpriteClips[1];
	HitBox movementHitBox;	// Hit box for movement
	HitBox projectileHitBox; // Hit box for projectiles
	HitBox damageHitBox; // Hit box for damage
	
	Inventory inventory;

private:
    float dx = 0; // delta x coordinat
    float dy = 0; //delta ycoordinate
    double angle = 0.0;
    int velocity = 500; // velocity of Marine movement
    int health = 100;
    int state; //used to select sprite to display

};

#endif
