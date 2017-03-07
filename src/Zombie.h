#ifndef ZOMBIE_H
#define ZOMBIE_H
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
#include "Movable.h"

const int ZOMBIE_HEIGHT = 125;
const int ZOMBIE_WIDTH = 75;

class Zombie : public Movable {
public:
    Zombie(int health = 100, int state = 0);
    virtual ~Zombie();

    //void move(float moveX, float moveY, CollisionHandler& ch); // Moves Zombie
    //void setDX(float px); //set delta x coordinate
    //void setDY(float py); //set delta y coordinate
    //void setVelocity(int pvel); // set velocity of Zombie movement
    //float getDX() const; // get delta x coordinate
    //float getDY() const; //get delta y coordinate
    //int getVelocity() const; // get velocity of Zombie movement
    int getHealth() const {return health;} // get health of Zombie
    void onCollision();
    void collidingProjectile(int damage);
    int getRandomAngle();
    void generateRandomMove();
	
private:
    int health;
    int state; //used to select sprite to display
};

#endif
