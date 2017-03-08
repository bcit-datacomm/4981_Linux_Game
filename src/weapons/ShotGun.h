/*
    Created by Maitiu Morton 2/8/2017
        Edited by DericM 3/8/2017
*/
#ifndef SHOTGUN_H
#define SHOTGUN_H
#include "InstantWeapon.h"
#include <string>

using std::string;

namespace ShotgunVars {
    const string TYPE  = "Shotgun";
    const int RANGE    = 50;
    const int DAMAGE   = 400;
    const int CLIP     = 2;
    const int CLIPMAX  = 2;
    const int AMMO     = 1000;
    const int AOE      = 1;
    const int RELOAD   = 3;
}

class ShotGun: public InstantWeapon {
public:

    ShotGun();
    ~ShotGun();
};

#endif
