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

/**
 * Programmers, Maitiu, Alex Zielinski
 * 
 * Revisions: 
 * Apr. 10, 2017, Alex Zielinski - Implemented pick up sound
 **/
bool Inventory::pickUp(int32_t weaponId, const float x, const float y) {
    if (current == 0) {
        logv(3, "Can't Swap default gun \n");
        return false;
    }

    //drop Current Weapon
    dropWeapon(x, y);

	// play medkit effect
	AudioManager::instance().playEffect(EFX_PPICK02);

    logv(3, "Picked up weapon\n");
    logv(3, "Swapped from %d ", weaponIds[current]);
    weaponIds[current] = weaponId;
    logv(3, "to %d\n\n", weaponIds[current]);
    return true;
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

void Inventory::useItem() {
    if (medkit != nullptr) {
		// play medkit effect
		AudioManager::instance().playEffect(EFX_MEDKIT);
        medkit->OnConsume();
        medkit = nullptr;
    }

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
 * DEVELOPER: Maitiu, Alex Zielinski
 * DESIGNER: Maitiu
 * DATE:      March 29 2017
 * Checks is current CSLot has Weapon then Checks its ammo and creates a weaponDrop and renders it.
 * 
 * Revisions:
 * Apr. 10, 2017 Alex Zielinski: implemented drop sound effect
 */
 void Inventory::dropWeapon(const float x, const float y) {
     if(current){
         Weapon *w = getCurrent();
         if (w) {
             if (w->getAmmo() > 0) {
				 // play drop sound effect
				 AudioManager::instance().playEffect(EFX_PDROP01);
                 GameManager::instance()->createWeaponDrop(x,y, weaponIds[current]);

             } else {
                 //delete weapon From Weapon Manager
                 GameManager::instance()->removeWeapon(weaponIds[current]);
             }
             weaponIds[current] = -1;
         }
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
 /**
 * Date: Mar. 30, 2017
 * Designer: Mark Chen
 * Programmer: Mark Chen
 * Function Interface: void makeTurretInv()
 * Description:
 * Switches the defaultGun to a turretGun.
 */
 void Inventory::makeTurretInv() {
     //Weapon *w = getCurrent();
     GameManager::instance()->removeWeapon(weaponIds[current]);
     TurretGun tGun(GameManager::instance()->generateID());
     weaponIds[0] = tGun.getID();
     GameManager::instance()->addWeapon(std::dynamic_pointer_cast<Weapon>(std::make_shared<TurretGun>(tGun)));
 }
 
