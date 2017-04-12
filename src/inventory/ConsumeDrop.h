#ifndef CONSUMEDROP_H
#define CONSUMEDROP_H
#include "../collision/HitBox.h"
#include "../basic/Entity.h"
#include "../collision/CollisionHandler.h"
#include "weapons/Weapon.h"
#include "Drop.h"
#include <SDL2/SDL.h>

class ConsumeDrop: public Drop{
public:
    ConsumeDrop(int32_t id, const SDL_Rect &dest, const SDL_Rect &pickupSize, int32_t cId);

    ~ConsumeDrop();

    int32_t getConsumeId()const{return consumableId;};
private:
    int32_t consumableId;
};



#endif
