#include "ConsumeDrop.h"
#include "../basic/Entity.h"
#include "../log/log.h"

/*
 * Created by Maitiu
 * Description: ConsumeDrop Ctro
 * Interface: ConsumeDrop::ConsumeDrop(int32_t id, const SDL_Rect &dest, const SDL_Rect &pickupSize): Drop(id, dest, pickupSize)
 *
 * id: The Index of the ConsumeDrop in the ConsumeDropManager
 * dest: the destination Rect of the ConsumeDrop
 * pickupSize: Thse Rect for the pickup hitbox
 *
 */
ConsumeDrop::ConsumeDrop(int32_t id, const SDL_Rect &dest, const SDL_Rect &pickupSize, int32_t cId): Drop(id, dest, pickupSize), consumableId(cId){
    logv("Created Consumable Drop\n");
}

ConsumeDrop::~ConsumeDrop(){
    logv("Destroyed Consumable Drop\n");
}
