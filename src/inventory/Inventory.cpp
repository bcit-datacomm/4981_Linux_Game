/*------------------------------------------------------------------------------
* Source: Inventory.cpp         
*
* Functions:
*
* Date: 
*
* Revisions: 
* Edited By : Tim Makimov on 2017/APR/05
*
* Designer: 
*
* Author: 
*
* Notes:
------------------------------------------------------------------------------*/

//created 2/5/17 Maitiu
#include <memory>

#include "Inventory.h"
#include "../game/GameManager.h"
#include "../log/log.h"

Inventory::Inventory(): defaultGun(GameManager::instance()->generateID()),
        tempZombieHand(GameManager::instance()->generateID()) {
    weaponIds[0] = defaultGun.getID();
    weaponIds[1] = -1;
    weaponIds[2] = -1;
    GameManager::instance()->addWeapon(std::dynamic_pointer_cast<Weapon>(std::make_shared<HandGun>(defaultGun)));

}


void Inventory::switchCurrent(const int slot) {
    if (current != slot) {
		// play menu click sound effect
		AudioManager::instance().playEffect(MENU_CLICK01);
        logv(3, "Switched to slot: %d\n", slot);
        current = slot;
    }
}

/*Created By Maitiu
 * Returns pointer to Weapon in current slot
 * If slot is empty it returns a null pointer
 */
Weapon* Inventory::getCurrent() const {
    if (weaponIds[current] >= 0) {
        return GameManager::instance()->getWeapon(weaponIds[current]).get();
    }
    return nullptr;
}

/**
 * Function: getWeaponFromInventory
 *
 * Date:
 * JF: April 4, 2017:
 *
 * Designer:
 * Jacob Frank
 *
 * Programmer:
 * Jacob Frank, Alex Zielinski
 * 
 * Modified:
 * Apr. 10, 2017 Alex Zielinski
 *
 * Interface: getWeaponFromInventory(int inventorySlot)
 *                  int inventorySlot: The inventory slot to retrieve the weapon from
 *
 * Returns: A Weapon from the inventory if one is available
 *
 * Notes:
 * Function, when called, retrieves the weapon from the inventory slot requested
 * 
 * Revisions:
 * Apr. 10, 2017 Alex Zielinski: implemented medkit sound effect
 */
Weapon* Inventory::getWeaponFromInventory(int inventorySlot) {
    if (weaponIds[inventorySlot] >= 0) {
        return GameManager::instance()->getWeapon(weaponIds[inventorySlot]).get();
    }
    return nullptr;
}

//Created By Maitiu
void Inventory::scrollCurrent(int direction) {
    int currentTime = SDL_GetTicks();

    if (currentTime > (slotScrollTick + scrollDelay)) {
        slotScrollTick = currentTime;
        direction += current;
        if (direction < 0) {
            current = 2;
        } else if (direction > 2) {
            current = 0;
        } else {
            current = direction;
        }
        logv(3, "Switched to slot:%d\n", current);
    }

}

 /**
  * Date: Mar. 31, 2017
  * Author: Mark Tattrie
  * Function Interface:  void Inventory::initZombie()
  * Description:
  * function to initialize a zombies inventory
  */
 void Inventory::initZombie(){
     weaponIds[0] = tempZombieHand.getID();
     weaponIds[1] = -1;
     weaponIds[2] = -1;
     GameManager::instance()->addWeapon(std::dynamic_pointer_cast<Weapon>
            (std::make_shared<ZombieHand>(tempZombieHand)));
 }
 
