/*
    Created by Maitiu Morton 2/8/2017
        Edited by DericM 3/8/2017
*/
#ifndef SHOTGUN_H
#define SHOTGUN_H
#include <string>

#include "InstantWeapon.h"
#include "../../audio/AudioManager.h"
#include "../../sprites/Renderer.h"
#include "../../sprites/SpriteTypes.h"

using std::string;

namespace ShotgunVars {
    const string TYPE           = "Shotgun";

    const string FIRESOUND      = EFX_WLRIFLE;
    const string HITSOUND       = "";
    const string RELOADSOUND    = EFX_WRELOAD01;
    const string EMPTYSOUND     = EFX_PDROP01;

    constexpr int RANGE         = 200;
    constexpr int DAMAGE        = 100;
    constexpr int AOE           = 1;
    constexpr int PENETRATION   = 1;

    constexpr int CLIP          = 2;
    constexpr int CLIPMAX       = 2;
    constexpr int AMMO          = 1000;

    constexpr int RELOADDELAY   = 3000;
    constexpr int FIREDELAY     = 1000;
}

class ShotGun: public InstantWeapon {
public:

    ShotGun(int32_t id);
    ~ShotGun() = default;

};

#endif
