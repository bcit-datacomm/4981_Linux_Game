/*
    Created by Maitiu Morton 2/8/2017
*/
#ifndef RIFLE_H
#define RIFLE_H
#include "InstantWeapon.h"
#include "../../audio/AudioManager.h"
#include <string>

using std::string;

namespace RifleVars {
    const string TYPE           = "Rifle";

    const string FIRESOUND      = EFX_WLRIFLE;
    const string HITSOUND       = "";
    const string RELOADSOUND    = "";
    const string EMPTYSOUND     = "";

    constexpr int RANGE         = 800;
    constexpr int DAMAGE        = 200;
    constexpr int AOE           = 1;

    constexpr int CLIP          = 2;
    constexpr int CLIPMAX       = 2;
    constexpr int AMMO          = 1000;
    
    constexpr int RELOADDELAY   = 3000;
    constexpr int FIREDELAY     = 1000;
}

class Rifle: public InstantWeapon {
public:

    Rifle();
    ~Rifle() = default;
};

#endif
