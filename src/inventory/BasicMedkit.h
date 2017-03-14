/*
    Created by Matt Goerwell 3/8/2017
*/
/*
    Created by Matthew Goerwell 3/8/2017
*/
#ifndef BASICMEDKIT_H
#define BASICMEDKIT_H
#include "Consumable.h"
#include <string>

/*
    This class represents a simple consumable in order to test basic consumable functionality.
    For the moment it does nothing, but in future builds it will add health to the player.
*/
class BasicMedkit: public Consumable {
public:
    //standard ctor
    BasicMedkit();
    //standard dtor
    ~BasicMedkit();
    //Consumable specific OnConsume Method
    void OnConsume() override;
};

#endif
