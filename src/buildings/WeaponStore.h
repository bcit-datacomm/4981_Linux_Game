#ifndef WEAPONSTORE_H
#define WEAPONSTORE_H

#include <utility>
#include <SDL2/SDL.h>
#include <memory>
#include "Store.h"


class WeaponStore: public Store{
public:
    WeaponStore(int32_t id, const SDL_Rect dest);

    ~WeaponStore();

    int32_t purchase(int num);//creates item and item drop for user
    int selectItem();//selected what item the user wants

private:

};

#endif
