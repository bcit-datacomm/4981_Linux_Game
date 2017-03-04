#include "Zombie.h"
#include <math.h>
#include <random>
#define PI 3.14159265
#define ZOMBIE_VELOCITY 200

Zombie::Zombie() : Movable(ZOMBIE_VELOCITY) {
    setAngle(getRandomAngle());
    printf("Create Zombie\n");
}

Zombie::~Zombie() {
    printf("Destory Zombie\n");
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
    int tx = getX()+50;
    int ty = getY()+50;
    if(tx<=0 || tx>=2000 || ty<=0 || ty>=2000){
        setAngle(getRandomAngle());
    }

    double cosVal = cos(getAngle()*PI/180.0);
    double sinVal = sin(getAngle()*PI/180.0);

    int x = getVelocity();
    int y = getVelocity();

    setDX(x*cosVal);
    setDY(y*sinVal);
}
