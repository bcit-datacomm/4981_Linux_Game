/*
    Created by Matt Goerwell 3/8/2017
*/
#include "BasicMedkit.h"
#include "../log/log.h"

BasicMedkit::BasicMedkit() : Consumable() {
    logv("BasicMedkit Created\n");
}

BasicMedkit::~BasicMedkit() {
    logv("BasicMedkit Destroyed\n");
}

void BasicMedkit::OnConsume() {
    logv("BasicMedkit Consumed\n");
}
