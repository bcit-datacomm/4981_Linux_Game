#include "WeaponDrop.h"

using namespace std;


WeaponDrop::WeaponDrop(Weapon &w): weapon(w){
    printf("Created Weapon Drop\n");
}


WeaponDrop::~WeaponDrop() {
    printf("Destory WeaponDrop\n");
}
void WeaponDrop::setPosition(int x, int y){
    xCoord = x;
    yCoord = y;
}

int WeaponDrop::getX() const{
    return xCoord;
}

int WeaponDrop::getY() const{
    return yCoord;
}

Weapon&  WeaponDrop::getWeapon(){
    return weapon;
}

void WeaponDrop::onCollision() {
    // Do nothing for now
}

void WeaponDrop::collidingProjectile(int damage) {
}
