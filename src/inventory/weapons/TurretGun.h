/*
    Created by Mark Chen 3/30/2017
*/
#ifndef TURRETGUN_H
#define TURRETGUN_H
#include <string>

#include "InstantWeapon.h"
#include "../../audio/AudioManager.h"
#include "../../sprites/Renderer.h"
#include "../../sprites/SpriteTypes.h"


using std::string;

namespace TurretGunVars {
    const string TYPE = "TurretGun";

    const string FIRESOUND = EFX_WTURRET;
    const string HITSOUND = "";
    const string RELOADSOUND = "";
    const string EMPTYSOUND = "";

    constexpr int RANGE = 400;
    constexpr int DAMAGE = 75;
    constexpr int AOE = 1;
    constexpr int PENETRATION = 1;
    constexpr int ACCURACY = 8;

    constexpr int CLIP = 200;
    constexpr int CLIPMAX = 200;
    constexpr int AMMO = 200;

    constexpr int RELOADDELAY = 100;
    constexpr int FIREDELAY = 250;
}


class TurretGun: public InstantWeapon {
public:

    TurretGun(int32_t id);
    ~TurretGun() = default;
};

#endif
