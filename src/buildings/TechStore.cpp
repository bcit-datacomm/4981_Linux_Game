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
        const GameHashMap<TEXTURES, int> i): Store(id, dest, pickupSize, screen, i, 2){

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
int TechStore::purchase(const int num, const int credits){

    GameManager *gm = GameManager::instance();
    if(gm->checkFreeDropPoints()){
        const int32_t dropPId = gm->getFreeDropPointId();
        if (gm->dropPointExists(dropPId)) {
            DropPoint dp = gm->getDropPoint(dropPId);
            switch(num){
                case 0:
                {
                    if(credits >= 500 && !gm->getPlayer().hasTurret()){
                        int32_t id = gm->createTurret(dp.getCoord().first, dp.getCoord().second);
                        //gm->getTurret(id).setDropZone(dropPId);
                        //gm->getPlayer().purchasedTurret();
                        return 500;
                    }
                    break;
                }
                case 1:
                {
                    if(credits >= 100){
                        int32_t id = gm->createBarricadeDrop(dp.getCoord().first, dp.getCoord().second);
                        gm->getBarricadeDrop(id).setDropPoint(dropPId);
                        return 100;
                    }
                    break;
                }
                default:
                    return -1;//does not exist
            }
        }

    }
    logv("NO OPEN DROP POINTS!!!\n");
    return -1;
}
