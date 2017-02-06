//created 2/5/17 Maitiu
#include "Inventory.h"

using namespace std;

Inventory::Inventory(){

}

Inventory::~Inventory(){
    
}

void Inventory::switchCurrent(int slot){
    printf("Switched to slot: %d\n", slot);
    current = slot;
}

void Inventory::pickUp(){
    weapons[current] = NULL;
}

Weapon* Inventory::getCurrent(){
    return weapons[current];
}
