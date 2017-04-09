/*
    Created by Deric Mccadden April 6 2017
*/
#ifndef PLASMADEAGLE_H
#define PLASMADEAGLE_H
#include <string>

#include "InstantWeapon.h"
#include "../../audio/AudioManager.h"
#include "../../sprites/Renderer.h"
#include "../../sprites/SpriteTypes.h"

using std::string;

namespace PlasmaDeagleVars {
    const string TYPE = "Plasma Deagle";

    const string FIRESOUND = EFX_WLRIFLE;
    const string HITSOUND = "";
    const string RELOADSOUND = EFX_WRELOAD01;
    const string EMPTYSOUND = EFX_PDROP01;

    constexpr int RANGE = 1000;
    constexpr int DAMAGE = 200;
    constexpr int AOE = 1;
    constexpr int PENETRATION = 5;
    constexpr int ACCURACY = 8;

    constexpr int CLIP = 8;
    constexpr int CLIPMAX = 8;
    constexpr int AMMO = 1000;

    constexpr int RELOADDELAY = 2000;
    constexpr int FIREDELAY = 200;
}

class PlasmaDeagle: public InstantWeapon {
public:

    PlasmaDeagle(const int32_t id);
    ~PlasmaDeagle() = default;
};

#endif
