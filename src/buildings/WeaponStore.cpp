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
WeaponStore::WeaponStore(int32_t id, const SDL_Rect dest): Store(id, dest){

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
    switch(num){
        case 1:
        {
            //make rifle
            Rifle r(id);
            break;
        }
        case 2:
        {
            //make shotgun
            ShotGun s(id);
            break;
        }
        case 3:
        {
            //make what ever
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

int WeaponStore::selectItem(){
    return 0;
}
