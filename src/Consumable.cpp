#include "Consumable.h"

Consumable::Consumable(LTexture gameTexture, LTexture interfaceTexture) : 
    inGameTexture(gameTexture), UiTexture(interfaceTexture) {
        printf("Create Consumable\n");
    }

Consumable::~Consumable(){
    printf("Destroy Consumable\n");
}