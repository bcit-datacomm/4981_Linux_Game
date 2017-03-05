#ifndef INVENTORY_H
#define INVENTORY_H
#include <SDL2/SDL.h>
#include <array>
#include "Weapon.h"
#include "HandGun.h"
#include "Rifle.h"
#include "ShotGun.h"

class Inventory {
public:
    void switchCurrent(int slot);//switches currently selected slot based on key input
    void pickUp();//picks up weapon in to current slot
    Weapon& getCurrent();//Returns current weapon
    void scrollCurrent(int direction);//switches current slot based on wheel scroll
    Inventory();
    ~Inventory();

private:
    int current = 0;//current weapon
    std::array<Weapon, 3> weapons;
    HandGun defaultGun;
    Rifle tempRifle;
    ShotGun tempShotGun;
    int slotScrollTick = 0;
    int scrollDelay = 200;
    int pickupTick = 0;
    int pickupDelay = 200;

};

#endif
