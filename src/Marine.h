#ifndef MARINE_H
#define MARINE_H
#include <string>
#include "LTexture.h"
#include "Entity.h"
#include "Movable.h"
#include "CollisionHandler.h"
#include "Inventory.h"
#include <math.h>
#include <vector>
#include <SDL2/SDL.h>
#include "Window.h"

#define MARINE_VELOCITY 500

const int MARINE_HEIGHT = 100;
const int MARINE_WIDTH = 100;

class Marine : public Movable {
public:
    void create(); //function displays character

    void onCollision();

    void collidingProjectile(int damage);

    Marine();
    virtual ~Marine();

    Inventory inventory;

private:
    int health = 100;
    int state; //used to select sprite to display

};

#endif
