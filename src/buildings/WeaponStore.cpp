#include "WeaponStore.h"
#include "../log/log.h"
#include "../game/GameManager.h"
#include "../inventory/weapons/Rifle.h"
#include "../inventory/weapons/ShotGun.h"
#include "../inventory/weapons/HandGun.h"
#include "DropPoint.h"
#include <memory>

/*
int32_t id of the Store
SDL_RECT dest the rect of the store
maitiu March 30*/
WeaponStore::WeaponStore(const int32_t id, const SDL_Rect dest, const SDL_Rect pickupSize): Store(id, dest, pickupSize){

}


/*maitiu March 30*/
WeaponStore::~WeaponStore(){
    logv("Destoryed Store");
}

/*
 *Created by maitiu March 30
 *Checks for available Drop Points and then creates weapon
 *int num what the player wants to purchase
 */
int32_t WeaponStore::purchase(const int num){

    GameManager *gm = GameManager::instance();
    if(gm->checkFreeDropPoints()){
        const int32_t dropPId = gm->getFreeDropPointId();
        const int32_t weaponId = createWeapon(num);

        DropPoint dp = gm->getDropPoint(dropPId);
        const float x = dp.getCoord().first;
        const float y = dp.getCoord().second;

        const int32_t wDropId = gm->createWeaponDrop(x, y, weaponId);

        logv("Purchased From WeaponStore\n");

        if(gm->weaponDropExists(wDropId)){
            gm->getWeaponDrop(wDropId).setDropPoint(dropPId);
        }
        return weaponId;
    }
    logv("NO OPEN DROP POINTS!!!\n");
    return -1;
}
/*
 *Created by maitiu March 30
 * Creates weapon
 */
int32_t WeaponStore::createWeapon(const int num){
    GameManager *gm = GameManager::instance();
    int32_t id = gm->generateID();
    switch(num){
        case 1:
            gm->addWeapon(std::dynamic_pointer_cast<Weapon>(std::make_shared<Rifle>(id)));
            break;
        case 2:
            gm->addWeapon(std::dynamic_pointer_cast<Weapon>(std::make_shared<ShotGun>(id)));
            break;
        default:
            return -1;//does not exist
    }

    return id;
}
