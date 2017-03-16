/*
    Created by Maitiu Morton 2/8/2017
        Edited by DericM 3/8/2017
*/
#ifndef HANDGUN_H
#define HANDGUN_H
#include "InstantWeapon.h"
#include "../../audio/AudioManager.h"
#include <string>

using std::string;

namespace HandgunVars {
    const string TYPE           = "HandGun";

    const string FIRESOUND      = EFX_WLPISTOL;
    const string HITSOUND       = "";
    const string RELOADSOUND    = "";
    const string EMPTYSOUND     = "";

    constexpr int RANGE         = 500;
    constexpr int DAMAGE        = 500;
    constexpr int AOE           = 1;

    constexpr int CLIP          = 5;
    constexpr int CLIPMAX       = 5;
    constexpr int AMMO          = 100;

    constexpr int RELOADDELAY   = 2000;
    constexpr int FIREDELAY     = 500;
}


class HandGun: public InstantWeapon {
public:

    HandGun();
    ~HandGun() = default;
};

#endif
