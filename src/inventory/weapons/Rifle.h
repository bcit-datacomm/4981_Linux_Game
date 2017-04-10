/*------------------------------------------------------------------------------
* Source: Rifle.h        
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
    const string TYPE = "Rifle";

    const string FIRESOUND = EFX_WLRIFLE;
    const string HITSOUND = "";
    const string RELOADSOUND = EFX_WRELOAD01;
    const string EMPTYSOUND = EFX_PDROP01;

    constexpr int RANGE = 800;
    constexpr int DAMAGE = 50;
    constexpr int AOE = 1;
    constexpr int PENETRATION = 3;
    constexpr int ACCURACY = 4;

    constexpr int CLIP = 50;
    constexpr int CLIPMAX = 50;
    constexpr int AMMO = 1000;

    constexpr int RELOADDELAY = 2000;
    constexpr int FIREDELAY = 180;

    constexpr int START_X = 891;
    constexpr int START_Y = 379;
}

class Rifle: public InstantWeapon {
public:

    Rifle(const int32_t id);
    ~Rifle() = default;
};

#endif
