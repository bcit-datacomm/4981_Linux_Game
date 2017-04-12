#include "HealthStore.h"
#include "../log/log.h"
#include "../game/GameManager.h"
#include "../inventory/Consumable.h"
#include "../inventory/BasicMedkit.h"
#include "DropPoint.h"
#include <memory>

/**
 * Date: April. 8, 2017
 * Modified: ----
 * Author: Maitiu Morton
 * Function Interface: HealthStore(const int32_t id, const SDL_Rect dest, const SDL_Rect pickupSize, SDL_Rect screen,
         const GameHashMap<TEXTURES, int> i)
 * Description:
 *     ctor for the Health.
 */
HealthStore::HealthStore(const int32_t id, const SDL_Rect dest, const SDL_Rect pickupSize, SDL_Rect screen,
        const GameHashMap<TEXTURES, int> i): Store(id, dest, pickupSize, screen, i, 3){

}


/**
 * Date: April. 8, 2017
 * Modified: ----
 * Author: Maitiu Morton
 * Function Interface: HealthStore::purchase(const int num, const int credits)
         int num what the player wants to purchase
         int credits: how much currency player has
 * Description:
 *     Checks for available Drop Points and then creates Healthkit with drops
 */
int HealthStore::purchase(const int num, const int credits){
    int32_t cId = -1;
    int32_t dropId;
    GameManager *gm = GameManager::instance();
    if(gm->checkFreeDropPoints()){
        const int32_t dropPId = gm->getFreeDropPointId();
        if (gm->dropPointExists(dropPId)) {
            DropPoint dp = gm->getDropPoint(dropPId);
            const float x = dp.getCoord().first;
            const float y = dp.getCoord().second;
            switch(num){
                case 1:
                    if(credits > HEALTH_PACK_PRICE){
                        cId = gm->generateID();
                        gm->addConsumable(std::dynamic_pointer_cast<Consumable>(std::make_shared<BasicMedkit>(gm->generateID())));
                        dropId = gm->createConsumeDrop(x, y, cId);
                        gm->getConsumeDrop(dropId).setDropPoint(dropPId);
                        return HEALTH_PACK_PRICE;
                    }
                    break;
            }
        }


    }
    logv("NO OPEN DROP POINTS!!!\n");
    return -1;
}
