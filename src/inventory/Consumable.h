/*
    Created by Matthew Goerwell 3/8/2017
*/
#ifndef Consumable_H
#define Consumable_H
#include <SDL2/SDL.h>
#include "../sprites/LTexture.h"
#include <string>


class Consumable {
public:
    //constructor that will assign the right textures to the consumable
    Consumable(LTexture gameTexture, LTexture interfaceTexture);
    //default constructor to be used for testing. Should be removed in final build.
    Consumable() = default;
    //Copy ctor
    Consumable(const Consumable& c);
    //standard destructor
    virtual ~Consumable();
    //abstarct method that must be filled in for each consumable type
    virtual void OnConsume() = 0; 

protected:
    //texture to be displayed when consumable is on the ground as part of a consumableDrop
    LTexture inGameTexture;
    //texture to be displayed when consumable is examined in inventory.
    LTexture UiTexture;
};

#endif
