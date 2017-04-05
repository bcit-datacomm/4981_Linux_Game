#ifndef INVENTORY_H
#define INVENTORY_H
#include <SDL2/SDL.h>
#include <array>
#include <memory>

#include "Consumable.h"
#include "BasicMedkit.h"
#include "weapons/Weapon.h"
#include "weapons/HandGun.h"
#include "weapons/Rifle.h"
#include "weapons/ShotGun.h"
#include "weapons/ZombieHand.h"

/*
 * Created By Maitiu Morton
 * THe inventory class acts as the marine's inventory. It sotres the ids for all the objects that the
 * Marine has equipped such as Weapons and Consumables.
 */
class Inventory {
public:
    void switchCurrent(int slot);//switches currently selected slot based on key input
    bool pickUp(int32_t weaponId, const float x, const float y);//picks up weapon in to current slot
    Weapon *getCurrent();//Returns current weapon
    void scrollCurrent(int direction);//switches current slot based on wheel scroll
    void useItem(); //uses current inventory item
    void dropWeapon(float x, float y);//creates WeaponDrop for Current Weapon before Dropping it
    int getCurretSlot() const {return current;}; //returns the current sloted selected int the inventory 
    Inventory();
    ~Inventory() = default;
    void initZombie();

private:
    int current = 0;//current weapon
    std::array<int32_t, 3> weaponIds;//array of weapon ids
    HandGun defaultGun;
    ZombieHand tempZombieHand;
    //temp for now, in the future this will simply be a pointer to a consumable which is null initially
    std::shared_ptr<BasicMedkit> medkit = std::shared_ptr<BasicMedkit>(new BasicMedkit());
    int slotScrollTick = 0;
    int scrollDelay = 200;
    int pickupTick = 0;
    int pickupDelay = 200;

};

#endif
