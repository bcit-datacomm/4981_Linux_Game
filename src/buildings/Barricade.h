#ifndef BARRICADE_H
#define BARRICADE_H
#include <string>
#include "../sprites/LTexture.h"
#include "../collision/HitBox.h"
#include "../buildings/Object.h"
#include "../collision/CollisionHandler.h"
#include "../inventory/Inventory.h"
#include <math.h>
#include <vector>
#include <SDL2/SDL.h>
#include "../view/Window.h"

constexpr int BARRICADE_WIDTH = 100;
constexpr int BARRICADE_HEIGHT = 100;

class Barricade : public Object {
public:
    Barricade(int health = 100, int state = 0, bool placeable = false, bool placed = false);
    virtual ~Barricade();

    void move(const float, const float, const float, const float, CollisionHandler&); // Moves Zombie
    void onCollision();
    void collidingProjectile(const int damage);
    bool isPlaceable();
    bool isPlaced();
    bool checkPlaceablePosition(const float,const float,const float,const float, CollisionHandler&);
    void placeBarricade();

private:
    int health;
    int state; //used to select sprite to display
    bool placeable;
    bool placed;
};

#endif
