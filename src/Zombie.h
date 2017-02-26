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

	void onCollision();

	void collidingProjectile(int damage);

	void generateRandomMove(); //randomly generate the zombie's movement

	int getRandomAngle(); //randomly generate the angle of zombie

	Zombie();
	virtual ~Zombie();

private:

    int health = 100;
    int state; //used to select sprite to display

};

#endif
