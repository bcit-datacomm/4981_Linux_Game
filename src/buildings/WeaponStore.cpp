#include "WeaponStore.h"
#include "../log/log.h"
#include "../game/GameManager.h"
#include "../inventory/weapons/Rifle.h"
#include "../inventory/weapons/ShotGun.h"
#include "../inventory/weapons/HandGun.h"
#include "../inventory/weapons/PlasmaDeagle.h"
#include "../inventory/weapons/Katana.h"
#include "../inventory/weapons/PhaseRepeater.h"
#include "../inventory/weapons/RailGun.h"
#include "../inventory/weapons/Type75.h"
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
}

/*
 *Created by maitiu March 30
 *Checks for available Drop Points and then creates weapon
 *int num what the player wants to purchase
 */
int WeaponStore::purchase(const int num, const int credits){

    GameManager *gm = GameManager::instance();
    if(gm->checkFreeDropPoints()){
        const int32_t dropPId = gm->getFreeDropPointId();
        if (gm->dropPointExists(dropPId)) {
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
            return gm->getWeapon(weaponId)->getPrice();
        }
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
        case 2:
        {
            PlasmaDeagle pd(id);
            if(credits >= pd.getPrice()){
                gm->addWeapon(std::dynamic_pointer_cast<Weapon>(std::make_shared<PlasmaDeagle>(pd)));
                return id;
            }
            break;
        }
        case 3:
        {
            RailGun ra(id);
            if(credits >= ra.getPrice()){
                gm->addWeapon(std::dynamic_pointer_cast<Weapon>(std::make_shared<RailGun>(ra)));
                return id;
            }
            break;
        }
        case 4:
        {
            Type75 t7(id);
            if(credits >= t7.getPrice()){
                gm->addWeapon(std::dynamic_pointer_cast<Weapon>(std::make_shared<Type75>(t7)));
                return id;
            }
            break;
        }
        case 5:
        {
            Katana k(id);
            if(credits >= k.getPrice()){
                gm->addWeapon(std::dynamic_pointer_cast<Weapon>(std::make_shared<Katana>(k)));
                return id;
            }
            break;
        }
        case 6:
        {
            PhaseRepeater pr(id);
            if(credits >= pr.getPrice()){
                gm->addWeapon(std::dynamic_pointer_cast<Weapon>(std::make_shared<PhaseRepeater>(pr)));
                return id;
            }
            break;
        }
        case 7:

        default:
            return -1;//does not exist
    }

    return -1;
}
