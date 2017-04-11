/*
    Created by Matthew Goerwell 3/8/2017
*/
#ifndef Consumable_H
#define Consumable_H

#include <SDL2/SDL.h>
#include <string>

#include "../log/log.h"


class Consumable {
public:
    //constructor that will assign the right textures to the consumable
    Consumable(int32_t i);
    //Copy ctor
    Consumable(const Consumable& c);
    //standard destructor
    virtual ~Consumable() = default;
    //abstarct method that must be filled in for each consumable type
    virtual void OnConsume() = 0;

    int32_t getId() const {return id;};
private:
    int32_t id;
};

#endif
