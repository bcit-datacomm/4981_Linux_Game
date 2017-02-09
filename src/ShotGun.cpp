/*
    Created by Maitiu Morton 2/8/2017
*/
#include "ShotGun.h"

ShotGun::ShotGun(){

    range = 100;
    damage = 500;
    clipMax = 2;
    clip = clipMax;
    ammo = 20;
    rAOE = 1;
    type = "Shotgun";
    reloadSpeed = 3;

}

ShotGun::~ShotGun(){

}
