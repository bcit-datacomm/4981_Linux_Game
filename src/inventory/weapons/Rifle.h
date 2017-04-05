/*
    Created by Maitiu Morton 2/8/2017
        Edited by MarkT 3/13/2017
*/
#ifndef RIFLE_H
#define RIFLE_H
#include <string>

#include "InstantWeapon.h"
#include "../../audio/AudioManager.h"
#include "../../sprites/Renderer.h"
#include "../../sprites/SpriteTypes.h"

using std::string;

namespace RifleVars {
    const string TYPE           = "Rifle";

    const string FIRESOUND      = EFX_WLRIFLE;
    const string HITSOUND       = "";
    const string RELOADSOUND    = EFX_WRELOAD01;
    const string EMPTYSOUND     = EFX_PDROP01;

    constexpr int RANGE         = 800;
    constexpr int DAMAGE        = 50;
    constexpr int AOE           = 1;
    constexpr int PENETRATION   = 3;

    constexpr int CLIP          = 2;
    constexpr int CLIPMAX       = 2;
    constexpr int AMMO          = 1000;

    constexpr int RELOADDELAY   = 3000;
    constexpr int FIREDELAY     = 1000;
}

class Rifle: public InstantWeapon {
public:

    Rifle(int32_t id);
    ~Rifle() = default;
};

#endif
