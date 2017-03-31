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
    Drop(int32_t id, const SDL_Rect &dest, const SDL_Rect &pickupSize);

    ~Drop();
};

#endif
