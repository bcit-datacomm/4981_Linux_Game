#ifndef BARRICADE_H
#define BARRICADE_H
#include <string>
#include "LTexture.h"
#include "HitBox.h"
#include "Object.h"
#include "CollisionHandler.h"
#include "Inventory.h"
#include <math.h>
#include <vector>
#include <SDL2/SDL.h>
#include "Window.h"

const int BARRICADE_WIDTH = 100;
const int BARRICADE_HEIGHT = 100;

class Barricade : public Object {
public:
    Barricade(int health = 100, int state = 0, bool booPlaceable = false, bool boolPlaced = false);
    virtual ~Barricade();

    void move(float, float, float, float, CollisionHandler&); // Moves Zombie
    void onCollision();
    void collidingProjectile(int damage);
    bool isPlaceable();
    bool isPlaced();
    bool checkPlaceablePosition(float, float, float, float, CollisionHandler&);
    void placeBarricade();

private:
    int health;
    int state; //used to select sprite to display
    bool boolPlaceable;
    bool boolPlaced;
};

#endif
