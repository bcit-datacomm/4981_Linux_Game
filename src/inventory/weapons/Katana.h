/*
    Created by Deric Mccadden 2/8/2017
*/
#ifndef KATANA_H
#define KATANA_H
#include <string>

#include "InstantWeapon.h"
#include "../../audio/AudioManager.h"
#include "../../sprites/Renderer.h"
#include "../../sprites/SpriteTypes.h"

using std::string;

namespace KatanaVars {
    const string TYPE = "Katana";

    const string FIRESOUND = EFX_WLPISTOL;
    const string HITSOUND = "";
    const string RELOADSOUND = EFX_WRELOAD01;
    const string EMPTYSOUND = EFX_PDROP01;

    constexpr int RANGE = 150;
    constexpr int DAMAGE = 100;
    constexpr int AOE = 1;
    constexpr int PENETRATION = 5;
    constexpr int ACCURACY = 2;

    constexpr int SPREAD_ANGLE = 100;
    constexpr int SPREAD_SHOTS = 10;

    constexpr int CLIP = 1;
    constexpr int CLIPMAX = 1;
    constexpr int AMMO = 1000000000;

    constexpr int RELOADDELAY = 0;
    constexpr int FIREDELAY = 400;

    constexpr int START_X = 891;
    constexpr int START_Y = 1502;
}


class Katana: public InstantWeapon {
public:

    Katana(const int32_t id);
    ~Katana() = default;

    bool fire(Movable& movable);
};

#endif
