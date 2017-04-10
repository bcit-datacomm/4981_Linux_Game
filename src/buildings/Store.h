#ifndef STORE_H
#define STORE_H

#include <utility>
#include <SDL2/SDL.h>
#include <memory>
#include <vector>
#include "Object.h"
#include "../game/GameHashMap.h"
#include "../sprites/SpriteTypes.h"
#include "StoreMenu.h"


class Store: public Object{
public:
    Store(const int32_t id, const SDL_Rect dest, const SDL_Rect pickupSize, SDL_Rect screen, const GameHashMap<TEXTURES, int> i);

    ~Store();

    virtual int32_t purchase(const int num, const int credits);//creates item and item drop for user
    std::pair<int,int> dropLocation();
    int32_t getVacantDropPoint();
    void addVacantDropPoint(const int32_t id);
    bool isOpen() const {return open;};
    void closeStore() {open = false;};
    void openStore() {open = true;};
    StoreMenu& getStoreMenu() {return menu;};

protected:
    int dropCols;
    int dropPoints;
    int size;
    bool open;
    StoreMenu menu;
};

#endif
