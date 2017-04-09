/*
    Created by Deric Mccadden April 6 2017
*/
#ifndef PHASEREPEATER_H
#define PHASEREPEATER_H
#include <string>

#include "InstantWeapon.h"
#include "../../audio/AudioManager.h"
#include "../../sprites/Renderer.h"
#include "../../sprites/SpriteTypes.h"

using std::string;

namespace PhaseRepeaterVars {
    const string TYPE = "Phase Repeater";

    const string FIRESOUND = EFX_WLRIFLE;
    const string HITSOUND = "";
    const string RELOADSOUND = EFX_WRELOAD01;
    const string EMPTYSOUND = EFX_PDROP01;

    constexpr int RANGE = 900;
    constexpr int DAMAGE = 50;
    constexpr int AOE = 1;
    constexpr int PENETRATION = 1;
    constexpr int ACCURACY = 8;

    constexpr int CLIP = 50;
    constexpr int CLIPMAX = 50;
    constexpr int AMMO = 1000;

    constexpr int RELOADDELAY = 1500;
    constexpr int FIREDELAY = 100;
}

class PhaseRepeater: public InstantWeapon {
public:

    PhaseRepeater(const int32_t id);
    ~PhaseRepeater() = default;
};

#endif
