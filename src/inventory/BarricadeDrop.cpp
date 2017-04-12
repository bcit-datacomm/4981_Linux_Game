#include "BarricadeDrop.h"
#include "../basic/Entity.h"
#include "../log/log.h"

/*
 * Created by Maitiu
 * Description: BarricadeDrop Ctro
 * Interface: BarricadeDrop::BarricadeDrop(int32_t id, const SDL_Rect &dest, const SDL_Rect &pickupSize): Drop(id, dest, pickupSize)
 *
 * id: The Index of the BarricadeDrop in the BarricadeDropManager
 * dest: the destination Rect of the BarricadeDrop
 * pickupSize: Thse Rect for the pickup hitbox
 *
 */
BarricadeDrop::BarricadeDrop(int32_t id, const SDL_Rect &dest, const SDL_Rect &pickupSize): Drop(id, dest, pickupSize){
    logv("Created barricade Drop\n");
}

BarricadeDrop::~BarricadeDrop(){
    logv("Destroyed barricade Drop\n");
}
