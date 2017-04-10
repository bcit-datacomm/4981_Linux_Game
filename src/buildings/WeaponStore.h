#ifndef WEAPONSTORE_H
#define WEAPONSTORE_H

#include <utility>
#include <SDL2/SDL.h>
#include <memory>
#include "Store.h"
#include "../game/GameHashMap.h"
#include "../sprites/SpriteTypes.h"

class WeaponStore: public Store{
public:
    WeaponStore(const int32_t id, const SDL_Rect dest, const SDL_Rect pickupSize, SDL_Rect screen,
            const GameHashMap<TEXTURES, int> i);

    ~WeaponStore();

    int32_t purchase(const int num, const int credits);//creates item and item drop for user
    int32_t createWeapon(const int num, const int credits);
    int getPrice()const {return price;};

private:

    int price;

};

#endif
