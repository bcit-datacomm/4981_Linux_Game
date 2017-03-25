#ifndef MARINE_H
#define MARINE_H

#include <string>
#include <math.h>
#include <vector>
#include <SDL2/SDL.h>

#include "../basic/Entity.h"
#include "../basic/Movable.h"
#include "../collision/CollisionHandler.h"
#include "../inventory/Inventory.h"
#include "../view/Window.h"

#define MARINE_VELOCITY 500

const int MARINE_HEIGHT = 100;
const int MARINE_WIDTH = 100;

class Marine : public Movable {
public:
    Marine(int32_t id, const SDL_Rect &dest, const SDL_Rect &movementSize,
        const SDL_Rect &projectileSize, const SDL_Rect &damageSize);
    virtual ~Marine();

    void create(); //function displays character
    void onCollision();
    void collidingProjectile(int damage);
    void fireWeapon();
    int32_t checkForPickUp();
    Inventory inventory;

private:
    int health = 100;
    int state; //used to select sprite to display
};

#endif
