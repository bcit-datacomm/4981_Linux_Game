//created 2/5/17 Maitiu
#include <memory>

#include "Inventory.h"
#include "../game/GameManager.h"
#include "../log/log.h"

Inventory::Inventory(): defaultGun(GameManager::instance()->generateID()){
    weaponIds[0] = defaultGun.getID();
    weaponIds[1] = -1;
    weaponIds[2] = -1;
    GameManager::instance()->addWeapon(std::dynamic_pointer_cast<Weapon>(std::make_shared<HandGun>(defaultGun)));

}

Inventory::~Inventory(){

}

void Inventory::switchCurrent(const int slot){
    if (current != slot) {
        logv("Switched to slot: %d\n", slot);
        current = slot;
    }
}
//Created By Maitiu
bool Inventory::pickUp(int32_t weaponId, float x, float y){
    if(current == 0){
        logv("Can't Swap default gun \n");
        return false;
    }


    Weapon *w = getCurrent();
    if(w != nullptr){
        dropWeapon(x, y);
    }
    logv("Picked up weapon\n");
    logv("Swapped from %d ", weaponIds[current]);
    weaponIds[current] = weaponId;
    logv("to %d\n\n", weaponIds[current]);
    return true;
}

Weapon* Inventory::getCurrent() {
    if(weaponIds[current] >= 0){
        return GameManager::instance()->getWeapon(weaponIds[current]).get();
    } else {
        return nullptr;
    }
}

void Inventory::useItem() {
    if (medkit != nullptr) {
        medkit->OnConsume();
        medkit = nullptr;
    }

}

void Inventory::scrollCurrent(int direction){
    int currentTime = SDL_GetTicks();

    if(currentTime > (slotScrollTick + scrollDelay)){
        slotScrollTick = currentTime;
        direction += current;
        if(direction < 0){
            current = 2;
        } else if (direction > 2){
            current = 0;
        } else {
            current = direction;
        }
        logv("Switched to slot:%d\n", current);
    }

}

/**
 * DEVELOPER: Maitiu
 * DESIGNER: Maitiu
 * DATE:      March 29 2017
 * Creates weapon Drop for Current Weapon
 */
 void Inventory::dropWeapon(float x, float y){

     GameManager::instance()->createWeaponDrop(x,y, weaponIds[current]);
 }
