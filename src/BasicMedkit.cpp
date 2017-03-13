/*
    Created by Matt Goerwell 3/8/2017
*/
#include "BasicMedkit.h"

BasicMedkit::BasicMedkit() : Consumable() {
    printf("BasicMedkit Created\n");
}

BasicMedkit::~BasicMedkit() {
    printf("BasicMedkit Destroyed\n");
}

void BasicMedkit::OnConsume() {
    printf("BasicMedkit Consumed\n");
}
