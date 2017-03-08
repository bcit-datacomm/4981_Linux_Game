/*
    Created by Matthew Goerwell 3/8/2017
*/
#ifndef Consumable_H
#define Consumable_H
#include <SDL2/SDL.h>
#include "LTexture.h"
#include <string>


class Consumable {
public:

    virtual void OnConsume() = 0;

    Consumable(LTexture gameTexture, LTexture interfaceTexture);

    Consumable(){};

    Consumable(const Consumable& c);

    virtual ~Consumable();

protected:
    LTexture inGameTexture;
    LTexture UiTexture;
};

#endif