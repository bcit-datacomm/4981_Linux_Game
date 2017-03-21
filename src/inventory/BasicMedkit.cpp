/*
    Created by Matt Goerwell 3/8/2017
*/
#include "BasicMedkit.h"
#include "../log/log.h"

BasicMedkit::BasicMedkit() : Consumable() {
    logi("BasicMedkit Created\n");
}

BasicMedkit::~BasicMedkit() {
    logi("BasicMedkit Destroyed\n");
}

void BasicMedkit::OnConsume() {
    logi("BasicMedkit Consumed\n");
}
