//created 2/5/17 Maitiu
#include <memory>
#include "Inventory.h"
#include "GameManager.h"

Inventory::Inventory(){
    weapons[0] = std::dynamic_pointer_cast<Weapon>(std::make_shared<HandGun>(defaultGun));
    weapons[1] = std::dynamic_pointer_cast<Weapon>(std::make_shared<Rifle>(tempRifle));
    weapons[2] = std::dynamic_pointer_cast<Weapon>(std::make_shared<ShotGun>(tempShotGun));
}

Inventory::~Inventory(){

}

void Inventory::switchCurrent(int slot){
    if (current != slot) {
        printf("Switched to %s  slot: %d\n", weapons[slot]->getType().c_str(), slot);
        current = slot;
    }
}

void Inventory::pickUp(int32_t weaponId){
    int currentTime = SDL_GetTicks();

    if(currentTime > (pickupTick + pickupDelay)){
        pickupTick = currentTime;
        if(current == 0){
            printf("Can't Swap default gun \n");
        } else{
            //Weapon w = GameManager::instance()->getWeapon(weaponId);
            printf("Swapped from %s ", weapons[current]->getType().c_str());

            //weapons[current] = std::dynamic_pointer_cast<Weapon>(std::make_shared<Rifle>(w));
            printf("to %s\n", weapons[current]->getType().c_str());

        }

    }
}

Weapon *Inventory::getCurrent() {
    return weapons[current].get();
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
        printf("Switched to %s slot:%d\n", weapons[current]->getType().c_str(), current);
    }

}
