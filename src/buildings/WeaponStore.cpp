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
SDL_Rect dest the Rect of the store
maitiu March 30*/
WeaponStore::WeaponStore(const int32_t id, const SDL_Rect dest, const SDL_Rect pickupSize, SDL_Rect screen,
        const GameHashMap<TEXTURES, int> i): Store(id, dest, pickupSize, screen, i, 1){

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
int32_t WeaponStore::purchase(const int num, const int credits){

    GameManager *gm = GameManager::instance();
    if(gm->checkFreeDropPoints()){
        const int32_t dropPId = gm->getFreeDropPointId();
        const int32_t weaponId = createWeapon(num, credits);
        if(weaponId < 0){
            return -1;
        }
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
int32_t WeaponStore::createWeapon(const int num, const int credits){
    GameManager *gm = GameManager::instance();
    int32_t id = gm->generateID();
    switch(num){
        case 0:
        {
            Rifle r(id);
            if(credits >= r.getPrice()){
                gm->addWeapon(std::dynamic_pointer_cast<Weapon>(std::make_shared<Rifle>(r)));
                return id;
            }
            break;
        }
        case 1:
        {
            ShotGun s(id);
            if(credits >= s.getPrice()){
                gm->addWeapon(std::dynamic_pointer_cast<Weapon>(std::make_shared<ShotGun>(s)));
                return id;
            }
            break;
        }
        default:
            return -1;//does not exist
    }

    return -1;
}
