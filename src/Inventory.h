#ifndef INVENTORY_H
#define INVENTORY_H
#include <SDL2/SDL.h>
#include <array>
#include <memory>
#include "weapons/Weapon.h"
#include "weapons/HandGun.h"
#include "weapons/Rifle.h"
#include "weapons/ShotGun.h"

class Inventory {
public:
    void switchCurrent(int slot);//switches currently selected slot based on key input
    void pickUp(int32_t weaponId);//picks up weapon in to current slot
    Weapon *getCurrent();//Returns current weapon
    void scrollCurrent(int direction);//switches current slot based on wheel scroll
    Inventory();
    ~Inventory();

private:
    int current = 0;//current weapon
    int32_t weaponIds[3];
    std::array<std::shared_ptr<Weapon>, 3> weapons;
    HandGun defaultGun;
    Rifle tempRifle;
    ShotGun tempShotGun;
    int slotScrollTick = 0;
    int scrollDelay = 200;
    int pickupTick = 0;
    int pickupDelay = 200;

};

#endif
