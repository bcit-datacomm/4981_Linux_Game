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
    const string TYPE = "Shotgun";
    constexpr int RANGE = 200;
    constexpr int DAMAGE = 400;
    constexpr int CLIP = 2;
    constexpr int CLIPMAX = 2;
    constexpr int AMMO = 1000;
    constexpr int AOE = 1;
    constexpr int RELOAD = 3;
    constexpr int FIRERATE = 600;
    constexpr bool READY = true;
}

class ShotGun: public InstantWeapon {
public:

    ShotGun();
    ~ShotGun() = default;
};

#endif
