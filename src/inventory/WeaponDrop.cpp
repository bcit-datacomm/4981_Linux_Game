#include "WeaponDrop.h"
#include "../basic/Entity.h"
#include "../log/log.h"
#include <atomic>

using namespace std;


WeaponDrop::WeaponDrop(int32_t id, const SDL_Rect &dest, const SDL_Rect &pickupSize,
        int32_t wId): Entity(id, dest, dest, pickupSize), weaponId(wId){

    logv("Created Weapon Drop\n");}

WeaponDrop::~WeaponDrop() {
    logv("Destroy WeaponDrop\n");
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

int32_t WeaponDrop::getWeaponId() const{
    return weaponId;
}

void WeaponDrop::onCollision() {
    // Do nothing for now
}

void WeaponDrop::collidingProjectile(int damage) {
}

int32_t WeaponDrop::getId() const {
    return Entity::getId();
}
