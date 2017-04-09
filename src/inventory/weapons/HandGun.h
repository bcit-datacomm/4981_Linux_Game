/*
    Created by Maitiu Morton 2/8/2017
        Edited by DericM 3/8/2017
        Edited by MarkT 3/13/2017
*/
#ifndef HANDGUN_H
#define HANDGUN_H
#include <string>

#include "InstantWeapon.h"
#include "../../audio/AudioManager.h"
#include "../../sprites/Renderer.h"
#include "../../sprites/SpriteTypes.h"

using std::string;

namespace HandgunVars {
    const string TYPE = "Handgun";

    const string FIRESOUND = EFX_WLPISTOL;
    const string HITSOUND = "";
    const string RELOADSOUND = EFX_WRELOAD01;
    const string EMPTYSOUND = EFX_PDROP01;

    constexpr int RANGE = 500;
    constexpr int DAMAGE = 200;
    constexpr int AOE = 1;
    constexpr int PENETRATION = 2;
    constexpr int ACCURACY = 2;

    constexpr int CLIP = 5;
    constexpr int CLIPMAX = 5;
    constexpr int AMMO = 100;

    constexpr int RELOADDELAY = 2000;
    constexpr int FIREDELAY = 500;
}


class HandGun: public InstantWeapon {
public:

    HandGun(const int32_t id);
    ~HandGun() = default;
};

#endif
