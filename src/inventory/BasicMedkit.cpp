/*
    Created by Matt Goerwell 3/8/2017
*/
#include "BasicMedkit.h"
#include "../log/log.h"
#include "../player/Marine.h"

BasicMedkit::BasicMedkit() : Consumable() {
    logv("BasicMedkit Created\n");
}

BasicMedkit::~BasicMedkit() {
    logv("BasicMedkit Destroyed\n");
}

void BasicMedkit::OnConsume(Marine& x) {
	x.setHealth(100);
    logv("BasicMedkit Consumed\n");
}
