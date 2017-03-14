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

constexpr int ZOMBIE_HEIGHT = 125;
constexpr int ZOMBIE_WIDTH = 75;

class Zombie : public Movable {
public:
    Zombie(int32_t id, const SDL_Rect &spriteSize, const SDL_Rect &movementSize, const SDL_Rect &projectileSize,
        const SDL_Rect &damageSize, const SDL_Rect &pickupSize, int health = 100, int state = 0);
    virtual ~Zombie();

    void onCollision();
    void collidingProjectile(int damage);
    void generateRandomMove(); //randomly generate the zombie's movement
    int getRandomAngle(); //randomly generate the angle of zombie
private:
    int health;
    int state; //used to select sprite to display

};

#endif
