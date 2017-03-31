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
WeaponStore::WeaponStore(int32_t id, const SDL_Rect dest, const SDL_Rect pickupSize): Store(id, dest, pickupSize){

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
int32_t WeaponStore::purchase(int num){

    GameManager *gm = GameManager::instance();
    if(gm->checkFreeDropPoints()){
        int32_t dropPId = gm->getFreeDropPointId();
        int32_t weaponId = createWeapon(num);
        int32_t wDropId;

        DropPoint dp = gm->getDropPoint(dropPId);
        float x = dp.getCoord().first;
        float y = dp.getCoord().second;

        logv("Purchased From WeaponStore\n");

        wDropId = gm->createWeaponDrop(x, y, weaponId);

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
 */
int WeaponStore::selectItem(){
    return 0;
}
/*
 *Created by maitiu March 30
 * Creates weapon
 */
int32_t WeaponStore::createWeapon(int num){
    GameManager *gm = GameManager::instance();
    int32_t id = gm->generateID();
    switch(num){
        case 1:
        {
            //make rifle
            Rifle r(id);
            gm->addWeapon(std::dynamic_pointer_cast<Weapon>(std::make_shared<Rifle>(r)));
            break;
        }
        case 2:
        {
            //make shotgun
            ShotGun s(id);
            gm->addWeapon(std::dynamic_pointer_cast<Weapon>(std::make_shared<ShotGun>(s)));
            break;
        }
        default:
        {
            //does not exist
            return -1;
            break;
        }
    }

    return id;
}
