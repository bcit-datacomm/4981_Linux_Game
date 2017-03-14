#include "WeaponDrop.h"
#include "../basic/Entity.h"
#include <atomic>

using namespace std;


WeaponDrop::WeaponDrop(int32_t wId): weaponId(wId){
    printf("Created Weapon Drop\n");
}

WeaponDrop::WeaponDrop(const WeaponDrop &wd): weaponId(wd.weaponId){

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

int32_t WeaponDrop::getWeaponId(){
    return weaponId;
}

void WeaponDrop::onCollision() {
    // Do nothing for now
}

void WeaponDrop::collidingProjectile(int damage) {
}

int32_t WeaponDrop::getId() {
    return Entity::getId();
}
