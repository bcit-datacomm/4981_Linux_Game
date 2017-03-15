#include "Zombie.h"
#include <math.h>
#include <random>
#include "GameManager.h"
#define PI 3.14159265
#define ZOMBIE_VELOCITY 200

Zombie::Zombie(int32_t id, const SDL_Rect dest, const SDL_Rect &spriteSize, const SDL_Rect &movementSize, const SDL_Rect &projectileSize,
        const SDL_Rect &damageSize, int health, int state):
        Movable(id, dest, spriteSize, movementSize, projectileSize, damageSize, ZOMBIE_VELOCITY), health(health), state(state) {
    setAngle(getRandomAngle());
    //printf("Create Zombie\n");
}

Zombie::~Zombie() {
    //printf("Destory Zombie\n");
}

void Zombie::onCollision() {
    // Do nothing for now
}

void Zombie::collidingProjectile(int damage) {
    health = health - damage;
}

int Zombie::getRandomAngle(){
    //random number generator
    std::random_device rd;
    std::mt19937 eng(rd());

    //range 0 to 360 degrees for zombie's angle
    std::uniform_int_distribution<> distr(0,360);

    return distr(eng);
}

void Zombie::generateRandomMove(){
    double cosVal = cos(getAngle()*PI/180.0);
    double sinVal = sin(getAngle()*PI/180.0);

    int x = getVelocity();
    int y = getVelocity();

    int tx = getX() + 20 * cosVal;
    int ty = getY() + 20 * sinVal;

    movementHitBox->move(tx,ty);
    projectileHitBox->move(tx,ty);

    CollisionHandler &ch = GameManager::instance()->getCollisionHandler();
    if(ch.detectMovementCollision(this)){
        setAngle(getRandomAngle());
        cosVal = cos(getAngle()*PI/180.0);
        sinVal = sin(getAngle()*PI/180.0);
    }
    movementHitBox->move(getX(),getY());
    projectileHitBox->move(getX(),getY());

    setDX(x*cosVal);
    setDY(y*sinVal);
 }
