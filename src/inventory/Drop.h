/*------------------------------------------------------------------------------
* Source: Drop.h         
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
#ifndef DROP_H
#define DROP_H
#include "../collision/HitBox.h"
#include "../basic/Entity.h"
#include "../collision/CollisionHandler.h"
#include "weapons/Weapon.h"
#include <SDL2/SDL.h>
#include <string>

class Drop: public Entity {
public:
    Drop(const int32_t id, const SDL_Rect &dest, const SDL_Rect &pickupSize);

    ~Drop() = default;

    void setDropPoint(const int32_t id) { dropPoint = id; };
    int32_t getDropPoint() const { return dropPoint; };
private:
    int32_t dropPoint;
};

#endif
