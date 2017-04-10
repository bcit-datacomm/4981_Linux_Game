/*
    Created by Deric Mccadden April 6 2017
*/
#ifndef RAILGUN_H
#define RAILGUN_H
#include <string>

#include "InstantWeapon.h"
#include "../../audio/AudioManager.h"
#include "../../sprites/Renderer.h"
#include "../../sprites/SpriteTypes.h"

using std::string;

namespace RailGunVars {
    const string TYPE = "Railgun";

    const string FIRESOUND = EFX_WLRIFLE;
    const string HITSOUND = "";
    const string RELOADSOUND = EFX_WRELOAD01;
    const string EMPTYSOUND = EFX_PDROP01;

    constexpr int RANGE = 3000;
    constexpr int DAMAGE = 300;
    constexpr int AOE = 1;
    constexpr int PENETRATION = 100;
    constexpr int ACCURACY = 4;

    constexpr int CLIP = 1;
    constexpr int CLIPMAX = 1;
    constexpr int AMMO = 1000;

    constexpr int RELOADDELAY = 1500;
    constexpr int FIREDELAY = 1500;

    constexpr int START_X = 891;
    constexpr int START_Y = 752;
}

class RailGun: public InstantWeapon {
public:

    RailGun(const int32_t id);
    ~RailGun() = default;
};

#endif
