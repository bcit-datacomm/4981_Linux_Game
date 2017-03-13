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
    Zombie(int health = 100, int state = 0);
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
