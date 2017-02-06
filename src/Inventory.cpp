//created 2/5/17 Maitiu
#include "Inventory.h"

Inventory::Inventory(){

}

Inventory::~Inventory(){
    
}

void Inventory::switchCurrent(int slot){
	if (current != slot) {
    printf("Switched to slot: %d\n", slot);
    current = slot;
	}
}

void Inventory::pickUp(){
    weapons[current] = NULL;
}

Weapon* Inventory::getCurrent(){
    return weapons[current];
}
