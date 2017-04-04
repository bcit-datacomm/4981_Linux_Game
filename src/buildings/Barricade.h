#ifndef BARRICADE_H
#define BARRICADE_H

#include <string>
#include <math.h>
#include <vector>
#include <SDL2/SDL.h>

#include "../collision/HitBox.h"
#include "../buildings/Object.h"
#include "../collision/CollisionHandler.h"
#include "../inventory/Inventory.h"
#include "../view/Window.h"

constexpr int BARRICADE_WIDTH = 100;
constexpr int BARRICADE_HEIGHT = 100;

class Barricade : public Object {
public:
    Barricade(const int32_t nid, const SDL_Rect& dest, const SDL_Rect& movementSize,
        const SDL_Rect& pickupSize, const int health = 100, const int state = 0, const bool placeable = false,
        const bool placed = false);
    virtual ~Barricade();

    void move(const float, const float, const float, const float, CollisionHandler&); // Moves Zombie
    void onCollision();
    void collidingProjectile(const int damage);
    bool isPlaceable() const;
    bool isPlaced() const;
    bool checkPlaceablePosition(const float, CollisionHandler&);
    void placeBarricade();

private:
    int health;
    int state; //used to select sprite to display
    bool placeable;
    bool placed;
};

#endif
