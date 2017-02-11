//created 2/5/17 Maitiu
#include "Inventory.h"

Inventory::Inventory(){
	weapons[0] = &defaultGun;
	weapons[1] = &tempRifle;
	weapons[2] = &tempShotGun;

}

Inventory::~Inventory(){

}

void Inventory::switchCurrent(int slot){
	if (current != slot) {

		printf("Switched to %s  slot: %d\n", weapons[slot]->getType().c_str(), slot);
	    current = slot;
	}
}

void Inventory::pickUp(){
    weapons[current] = NULL;
}

Weapon* Inventory::getCurrent(){
    return weapons[current];
}

void Inventory::scrollCurrent(int direction){
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
