#include "Zombie.h"
#include <math.h>
#include <random>
#include "../game/GameManager.h"
#include "../log/log.h"
#define PI 3.14159265
#define ZOMBIE_VELOCITY 200

Zombie::Zombie(int health, int state) : Movable(ZOMBIE_VELOCITY), health(health), state(state) {
    setAngle(getRandomAngle());
    //logv("Create Zombie\n");
}

Zombie::~Zombie() {
    //logv("Destroy Zombie\n");
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

    const int x = getVelocity();
    const int y = getVelocity();

    const int tx = getX() + 20 * cosVal;
    const int ty = getY() + 20 * sinVal;

    movementHitBox->move(tx,ty);
    projectileHitBox->move(tx,ty);

    CollisionHandler &ch = GameManager::instance()->getCollisionHandler();
    if(ch.detectMovementCollision(this)){
        setAngle(getRandomAngle());
        cosVal = cos(getAngle() * PI / 180.0);
        sinVal = sin(getAngle() * PI / 180.0);
    }
    movementHitBox->move(getX(),getY());
    projectileHitBox->move(getX(),getY());

    setDX(x*cosVal);
    setDY(y*sinVal);
 }
