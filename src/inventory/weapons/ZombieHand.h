/*------------------------------------------------------------------------------
* Source: ZombieHand.h       
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

/**
    Created by Mark Tattrie 3/28/2017
*/
#ifndef ZOMBIEHAND_H
#define ZOMBIEHAND_H

#include "MeleeWeapon.h"
#include "../../audio/AudioManager.h"
#include <string>
#include "../../log/log.h"
#include "../../audio/AudioManager.h"
#include "../../sprites/Renderer.h"
#include "../../sprites/SpriteTypes.h"


namespace ZombieHandVars {
    const std::string TYPE = "ZombieHand";

    const std::string FIRESOUND = "";
    const std::string HITSOUND = "";
    const std::string RELOADSOUND = EFX_WRELOAD01;
    const std::string EMPTYSOUND = EFX_PDROP01;

    constexpr int RANGE = 100;
    constexpr int DAMAGE = 20;
    constexpr int AOE = 1;
    constexpr int PENETRATION = 1;
    constexpr int ACCURACY = 4;

    constexpr int CLIP = 10000;
    constexpr int CLIPMAX = 10000;
    constexpr int AMMO = 10000;

    constexpr int RELOADDELAY = 0;
    constexpr int FIREDELAY = 300;
}

class ZombieHand: public MeleeWeapon {
public:

    ZombieHand(const int32_t id);
    ~ZombieHand() = default;
};

#endif
