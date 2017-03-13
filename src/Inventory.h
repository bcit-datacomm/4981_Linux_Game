#ifndef INVENTORY_H
#define INVENTORY_H
#include <SDL2/SDL.h>
#include <array>
#include <memory>
#include "Weapon.h"
#include "HandGun.h"
#include "Rifle.h"
#include "ShotGun.h"
#include "Consumable.h"
#include "BasicMedkit.h"
#include "weapons/Weapon.h"
#include "weapons/HandGun.h"
#include "weapons/Rifle.h"
#include "weapons/ShotGun.h"


class Inventory {
public:
    void switchCurrent(int slot);//switches currently selected slot based on key input
    void pickUp();//picks up weapon in to current slot
    Weapon *getCurrent();//Returns current weapon
    void scrollCurrent(int direction);//switches current slot based on wheel scroll
    void useItem(); //uses current inventory item
    Inventory();
    ~Inventory();

private:
    int current = 0;//current weapon
    std::array<std::shared_ptr<Weapon>, 3> weapons;
    HandGun defaultGun;
    Rifle tempRifle;
    ShotGun tempShotGun;
    //temp for now, in the future this will simply be a pointer to a consumable which is null initially
    std::shared_ptr<BasicMedkit> medkit = std::shared_ptr<BasicMedkit>(new BasicMedkit());
    int slotScrollTick = 0;
    int scrollDelay = 200;
    int pickupTick = 0;
    int pickupDelay = 200;

};

#endif
