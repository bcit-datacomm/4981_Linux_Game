
#ifndef BASICMEDKIT_H
#define BASICMEDKIT_H
#include <string>

#include "Consumable.h"

/*
 *  Created by Matthew Goerwell 3/8/2017
 *  This is meant to serve as our basic consumable, to provide an example of how others would be 
 *  implemented. It represents a simple medkit that will heal the player to full health when used.
 */
class BasicMedkit: public Consumable {
public:
    //standard ctor
    BasicMedkit();
    //standard dtor
    ~BasicMedkit();
    //Consumable specific OnConsume Method
    void OnConsume(Marine& marine) override;
};

#endif
