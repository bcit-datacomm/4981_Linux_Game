/*
    Created by Deric Mccadden Apr/6/2017
*/
#ifndef SHOTGUN_H
#define SHOTGUN_H
#include <string>

#include "InstantWeapon.h"
#include "../../audio/AudioManager.h"
#include "../../sprites/Renderer.h"
#include "../../sprites/SpriteTypes.h"

using std::string;

namespace Type75Vars {
    const string TYPE = "Type75";

    const string FIRESOUND = EFX_WLRIFLE;
    const string HITSOUND = "";
    const string RELOADSOUND = EFX_WRELOAD01;
    const string EMPTYSOUND = EFX_PDROP01;

    constexpr int RANGE = 2000;
    constexpr int DAMAGE = 300;
    constexpr int AOE = 1;
    constexpr int PENETRATION = 3;
    constexpr int ACCURACY = 6;

    constexpr int CLIP = 5;
    constexpr int CLIPMAX = 5;
    constexpr int AMMO = 1000;

    constexpr int RELOADDELAY = 3000;
    constexpr int FIREDELAY = 1000;
}

class Type75: public InstantWeapon {
public:

    Type75(const int32_t id);
    ~Type75() = default;
};

#endif
