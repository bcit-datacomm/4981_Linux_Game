#ifndef TECHSTORE_H
#define TECHSTORE_H

#include <utility>
#include <SDL2/SDL.h>
#include <memory>
#include "Store.h"
#include "../game/GameHashMap.h"
#include "../sprites/SpriteTypes.h"

class TechStore: public Store{
public:
    TechStore(const int32_t id, const SDL_Rect dest, const SDL_Rect pickupSize, SDL_Rect screen,
            const GameHashMap<TEXTURES, int> i);

    ~TechStore();

    int purchase(const int num, const int credits);//creates item and item drop for user

private:

};

#endif
