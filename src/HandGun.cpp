/*
    Created by Maitiu Morton 2/8/2017
*/
#include "HandGun.h"

HandGun::HandGun(){

    range = 300;
    damage = 10;
    clipMax = 5;
    clip = clipMax;
    ammo = -1;
    rAOE = 1;
    type = "Hand Gun";
    reloadSpeed = 2;

}

HandGun::~HandGun(){

}
