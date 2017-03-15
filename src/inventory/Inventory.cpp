//created 2/5/17 Maitiu
#include <memory>
#include "Inventory.h"
#include "../game/GameManager.h"

Inventory::Inventory(){
    weapons[0] = std::dynamic_pointer_cast<Weapon>(std::make_shared<HandGun>(defaultGun));
    weapons[1] = std::dynamic_pointer_cast<Weapon>(std::make_shared<Rifle>(tempRifle));
    weapons[2] = std::dynamic_pointer_cast<Weapon>(std::make_shared<ShotGun>(tempShotGun));
    weaponIds[0] = defaultGun.getId();
    weaponIds[1] = -1;
    weaponIds[2] = -1;
    GameManager::instance()->addWeapon(std::dynamic_pointer_cast<Weapon>(std::make_shared<HandGun>(defaultGun)));

}

Inventory::~Inventory(){

}

void Inventory::switchCurrent(int slot){
    if (current != slot) {
        printf("Switched to slot: %d\n", slot);
        current = slot;
    }
}

bool Inventory::pickUp(int32_t weaponId){
    if(current == 0){
        printf("Can't Swap default gun \n");
        return false;
    }
    printf("Picked up weapon\n");
    printf("Swapped from %d ", weaponIds[current]);

    weaponIds[current] = weaponId;
    printf("to %d\n", weaponIds[current]);
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
        printf("Switched to slot:%d\n", current);
    }

}
