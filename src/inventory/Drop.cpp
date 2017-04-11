/*------------------------------------------------------------------------------
* Source: Drop.cpp         
*
* Functions:
*
* Date: 
*
* Revisions: 
* Edited By : Tim Makimov on 2017/APR/05
*
* Designer: 
*
* Author: 
*
* Notes:
------------------------------------------------------------------------------*/
#include "Drop.h"
#include "../log/log.h"
/*
 * Created by Maitiu
 * Description: Drop Ctro
 * Interface: Drop::Drop(int32_t id, const SDL_Rect &dest, const SDL_Rect &pickupSize): Drop(id, dest, pickupSize)
 *
 * id: The Index of the Drop in the DropManager
 * dest: the destination Rect of the Drop
 * pickupSize: Thse Rect for the pickup hitbox
 *
 */
Drop::Drop(const int32_t id, const SDL_Rect &dest, const SDL_Rect &pickupSize): Entity(id, dest, dest, pickupSize), dropPoint(-1){

    logv("Created Drop\n");
}
