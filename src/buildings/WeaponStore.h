#ifndef WEAPONSTORE_H
#define WEAPONSTORE_H

#include <utility>
#include <SDL2/SDL.h>
#include <memory>
#include "Store.h"


class WeaponStore: public Store{
public:
    WeaponStore(const int32_t id, const SDL_Rect dest, const SDL_Rect pickupSize);

    ~WeaponStore();

    int32_t purchase(const int num);//creates item and item drop for user
    int32_t createWeapon(const int num);

private:

};

#endif
