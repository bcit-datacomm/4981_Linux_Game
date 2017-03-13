#include "Consumable.h"
/*
    Created by Matthew Goerwell 3/8/2017
*/

//Assignment constructor that prints out when it's used for record tracking.
Consumable::Consumable(LTexture gameTexture, LTexture interfaceTexture) : 
        inGameTexture(gameTexture), UiTexture(interfaceTexture) {
    printf("Create Consumable\n");
}
//Destructor. Prints out use for testing purposes
Consumable::~Consumable(){
    printf("Destroy Consumable\n");
}
