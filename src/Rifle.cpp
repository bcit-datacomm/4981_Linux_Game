/*
    Created by Maitiu Morton 2/8/2017
*/
#include "Rifle.h"

using namespace std;

Rifle::Rifle(){

    range = 1000;
    damage = 100;
    clipMax = 1;
    clip = clipMax;
    ammo = 20;
    rAOE = 1;
    type = "Rifle";
    reloadSpeed = 5;
}

Rifle::~Rifle(){

}
