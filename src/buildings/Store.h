#ifndef STORE_H
#define STORE_H

#include <utility>
#include <SDL2/SDL.h>
#include <memory>
#include <vector>
#include "Object.h"


class Store: public Object{
public:
    Store(const int32_t id, const SDL_Rect dest, const SDL_Rect pickupSize);

    ~Store();

    virtual int32_t purchase(const int num);//creates item and item drop for user
    std::pair<int,int> dropLocation();
    int32_t getVacantDropPoint();
    void addVacantDropPoint(const int32_t id);

protected:
    int dropCols;
    int dropPoints;
    int size;
};

#endif
