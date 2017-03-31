#include "WeaponStore.h"
#include "../log/log.h"
#include "../game/GameManager.h"
#include "../inventory/weapons/Rifle.h"
#include "../inventory/weapons/ShotGun.h"
#include "../inventory/weapons/HandGun.h"
#include <memory>

/*
int32_t id of the Store
SDL_RECT dest the rect of the store
maitiu March 16*/
WeaponStore::WeaponStore(int32_t id, const SDL_Rect dest, const SDL_Rect pickupSize): Store(id, dest, pickupSize){

}


/*maitiu March 16*/
WeaponStore::~WeaponStore(){
    logv("Destoryed Store");
}

/*makes the weapon that the user wishes to purchase
int num what the player wants to purchase
maitiu March 16*/
int32_t WeaponStore::purchase(int num){
    GameManager *gm = GameManager::instance();
    int32_t id = gm->generateID();
    logv("Purchased From WeaponStore\n");
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
    gm->createWeaponDrop(300, 500, id);
    return id;
}

int WeaponStore::selectItem(){
    return 0;
}
