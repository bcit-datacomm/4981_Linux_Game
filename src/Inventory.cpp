//created 2/5/17 Maitiu
#include "Inventory.h"

Inventory::Inventory(){
    weapons[0] = defaultGun;
    weapons[1] = tempRifle;
    weapons[2] = tempShotGun;
}

Inventory::~Inventory(){

}

void Inventory::switchCurrent(int slot){
    if (current != slot) {

        printf("Switched to %s  slot: %d\n", weapons[slot].getType().c_str(), slot);
        current = slot;
    }
}

void Inventory::pickUp(){
    int currentTime = SDL_GetTicks();

    if(currentTime > (pickupTick + pickupDelay)){
        pickupTick = currentTime;
        if(current == 0){
            printf("Can't Swap default gun \n");
        } else{

            int randGun = rand() % 2 + 1;
            if(randGun == 1){
                printf("Swapped from %s ", weapons[current].getType().c_str());
                weapons[current] = tempRifle;
                printf("to %s\n", weapons[current].getType().c_str());
            } else if(randGun == 2){
                printf("Swapped from %s ", weapons[current].getType().c_str());
                weapons[current] = tempShotGun;
                printf("to %s\n", weapons[current].getType().c_str());
            }
        }

    }
}

Weapon& Inventory::getCurrent(){
    return weapons[current];
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
        printf("Switched to %s slot:%d\n", weapons[current].getType().c_str(), current);
    }

}
