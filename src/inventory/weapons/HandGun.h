/*
    Created by Maitiu Morton 2/8/2017
        Edited by DericM 3/8/2017
*/
#ifndef HANDGUN_H
#define HANDGUN_H
#include "InstantWeapon.h"
#include <string>

using std::string;

namespace HandgunVars {
    const string TYPE = "HandGun";
    constexpr int RANGE = 500;
    constexpr int DAMAGE = 500;
    constexpr int CLIP  = 2;
    constexpr int CLIPMAX = 2;
    constexpr int AMMO = 50;
    constexpr int AOE = 1;
    constexpr int RELOAD = 3;
    constexpr int FIRERATE = 1000;
    constexpr bool READY = true;
}


class HandGun: public InstantWeapon {
public:

    HandGun();
    ~HandGun() = default;
};

#endif
