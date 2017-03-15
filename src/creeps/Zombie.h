#ifndef ZOMBIE_H
#define ZOMBIE_H
#include <string>
#include "../sprites/LTexture.h"
#include "../collision/HitBox.h"
#include "../basic/Entity.h"
#include "../collision/CollisionHandler.h"
#include "../inventory/Inventory.h"
#include <math.h>
#include <vector>
#include <SDL2/SDL.h>
#include "../view/Window.h"
#include "../basic/Movable.h"

constexpr static int ZOMBIE_HEIGHT = 125;
constexpr static int ZOMBIE_WIDTH = 75;

class Zombie : public Movable {
public:
    Zombie(int health = 100, int state = 0);
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
