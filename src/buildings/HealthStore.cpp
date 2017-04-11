#include "HealthStore.h"
#include "../log/log.h"
#include "../game/GameManager.h"
#include "../inventory/Consumable.h"
#include "../inventory/BasicMedkit.h"
#include "DropPoint.h"
#include <memory>

/*
int32_t id of the Store
SDL_Rect dest the Rect of the store
maitiu March 30*/
HealthStore::HealthStore(const int32_t id, const SDL_Rect dest, const SDL_Rect pickupSize, SDL_Rect screen,
        const GameHashMap<TEXTURES, int> i): Store(id, dest, pickupSize, screen, i, 3){

}


/*maitiu March 30*/
HealthStore::~HealthStore(){
    logv("Destoryed Store");
}

/*
 *Created by maitiu March 30
 *Checks for available Drop Points and then creates Health
 *int num what the player wants to purchase
 */
int32_t HealthStore::purchase(const int num, const int credits){
    int32_t cId = -1;
    int32_t dropId;
    GameManager *gm = GameManager::instance();
    if(gm->checkFreeDropPoints()){
        const int32_t dropPId = gm->getFreeDropPointId();

        DropPoint dp = gm->getDropPoint(dropPId);
        const float x = dp.getCoord().first;
        const float y = dp.getCoord().second;
        switch(num){
            case 1:
                if(credits > 20){
                    cId = gm->generateID();
                    gm->addConsumable(std::dynamic_pointer_cast<Consumable>(std::make_shared<BasicMedkit>(gm->generateID())));
                    gm->createConsumeDrop(x, y, cId);
                }
                break;
        }

    }
    logv("NO OPEN DROP POINTS!!!\n");
    return -1;
}
