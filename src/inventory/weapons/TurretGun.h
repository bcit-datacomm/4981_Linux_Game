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

    const string FIRESOUND = EFX_WTURRET01;
    const string HITSOUND = "";
    const string RELOADSOUND = "";
    const string EMPTYSOUND = "";

    constexpr int RANGE = 400;
    constexpr int DAMAGE = 75;
    constexpr int AOE = 1;
    constexpr int PENETRATION = 3;

    constexpr int CLIP = 10;
    constexpr int CLIPMAX = 10;
    constexpr int AMMO = 10;

    constexpr int RELOADDELAY = 100;
    constexpr int FIREDELAY = 1500;
}


class TurretGun: public InstantWeapon {
public:

    TurretGun(int32_t id);
    ~TurretGun() = default;
};

#endif
