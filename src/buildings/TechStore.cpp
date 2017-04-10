#include "TechStore.h"
#include "../log/log.h"
#include "../game/GameManager.h"
#include "../turrets/Turret.h"
#include "Barricade.h"
#include "DropPoint.h"
#include <memory>

/*
int32_t id of the Store
SDL_Rect dest the Rect of the store
maitiu March 30*/
TechStore::TechStore(const int32_t id, const SDL_Rect dest, const SDL_Rect pickupSize, SDL_Rect screen,
        const GameHashMap<TEXTURES, int> i): Store(id, dest, pickupSize, screen, i){

}


/*maitiu March 30*/
TechStore::~TechStore(){
    logv("Destoryed Store");
}

/*
 *Created by maitiu March 30
 *Checks for available Drop Points and then creates Tech
 *int num what the player wants to purchase
 */
int32_t TechStore::purchase(const int num, const int credits){

    GameManager *gm = GameManager::instance();
    if(gm->checkFreeDropPoints()){

    }
    logv("NO OPEN DROP POINTS!!!\n");
    return -1;
}
/*
 *Created by maitiu March 30
 * Creates Tech
 */
int32_t TechStore::createTech(const int num, const int credits){
    GameManager *gm = GameManager::instance();
    int32_t id = gm->generateID();
    switch(num){
        case 0:
        {
            break;
        }
        case 1:
        {
            break;
        }
        default:
            return -1;//does not exist
    }

    return -1;
}
