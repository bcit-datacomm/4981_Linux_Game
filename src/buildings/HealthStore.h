#ifndef HEALTHSTORE_H
#define HEALTHSTORE_H

#include <utility>
#include <SDL2/SDL.h>
#include <memory>
#include "Store.h"
#include "../game/GameHashMap.h"
#include "../sprites/SpriteTypes.h"

class HealthStore: public Store{
public:
    HealthStore(const int32_t id, const SDL_Rect dest, const SDL_Rect pickupSize, SDL_Rect screen,
            const GameHashMap<TEXTURES, int> i);

    ~HealthStore();

    int32_t purchase(const int num, const int credits);//creates item and item drop for user
    int32_t createHealth(const int num, const int credits);

private:

};

#endif
