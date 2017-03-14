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
    const string TYPE       = "HandGun";
    const int RANGE         = 500;
    const int DAMAGE        = 500;
    const int CLIP          = 2;
    const int CLIPMAX       = 2;
    const int AMMO          = 50;
    const int AOE           = 1;
    const int RELOAD        = 3;
    const int FIRERATE      = 1000;
    constexpr bool READY    = true;
}


class HandGun: public InstantWeapon {
public:

    HandGun();
    ~HandGun() = default;
};

#endif
