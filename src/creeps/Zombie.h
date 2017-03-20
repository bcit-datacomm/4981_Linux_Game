#ifndef ZOMBIE_H
#define ZOMBIE_H

#include <string>
#include <math.h>
#include <vector>
#include <SDL2/SDL.h>

#include "../collision/HitBox.h"
#include "../basic/Entity.h"
#include "../collision/CollisionHandler.h"
#include "../inventory/Inventory.h"
#include "../collision/Quadtree.h"
#include "../view/Window.h"
#include "../basic/Movable.h"

constexpr static int ZOMBIE_HEIGHT = 125;
constexpr static int ZOMBIE_WIDTH = 75;

class Zombie : public Movable {
public:
    Zombie(int32_t id, const SDL_Rect &dest, const SDL_Rect &movementSize, const SDL_Rect &projectileSize,
        const SDL_Rect &damageSize, int health = 100, int state = 0);
    virtual ~Zombie();


    void onCollision();
    void collidingProjectile(int damage);
    void generateRandomMove(); //randomly generate the zombie's movement
    int getRandomAngle(); //randomly generate the angle of zombie

    void setHealth(const int& hlth) {health = hlth;};
private:
    int health;
    int state; //used to select sprite to display

};

#endif
