#include "Consumable.h"
#include "../log/log.h"
/*
    Created by Matthew Goerwell 3/8/2017
*/

//Assignment constructor that prints out when it's used for record tracking.
Consumable::Consumable(LTexture gameTexture, LTexture interfaceTexture) : 
        inGameTexture(gameTexture), UiTexture(interfaceTexture) {
    logv("Create Consumable\n");
}
//Destructor. Prints out use for testing purposes
Consumable::~Consumable(){
    logv("Destroy Consumable\n");
}
