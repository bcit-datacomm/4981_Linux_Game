/*------------------------------------------------------------------------------
* Source: ShotGun.h        
*
* Functions:
*
* Date: 
*
* Revisions: 
* Edited By : Tim Makimov on 2017/APR/10
*
* Designer: 
*
* Author: 
*
* Notes:
------------------------------------------------------------------------------*/

/*
    Created by Maitiu Morton 2/8/2017
        Edited by DericM 3/8/2017
        Edited by MarkT 3/13/2017
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
    const string TYPE = "Shotgun";

    const string FIRESOUND = EFX_WLRIFLE;
    const string HITSOUND = "";
    const string RELOADSOUND = EFX_WRELOAD01;
    const string EMPTYSOUND = EFX_PDROP01;

    constexpr int RANGE = 700;
    constexpr int DAMAGE = 50;
    constexpr int AOE = 1;
    constexpr int PENETRATION = 0;
    constexpr int ACCURACY = 6;

    constexpr int SPREAD_ANGLE = 30;
    constexpr int SPREAD_SHOTS = 5;

    constexpr int CLIP = 5;
    constexpr int CLIPMAX = 5;
    constexpr int AMMO = 1000;

    constexpr int RELOADDELAY = 3000;
    constexpr int FIREDELAY = 1000;

    constexpr int START_X = 891;
    constexpr int START_Y = 942;
}

class ShotGun: public InstantWeapon {
public:

    ShotGun(const int32_t id);
    ~ShotGun() = default;

    bool fire(Movable& movable);
};

#endif
