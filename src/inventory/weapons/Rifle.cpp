/*
    Created by Maitiu Morton 2/8/2017
        Edited by DericM 3/8/2017
        Edited by Mark Tattrie 3/13/2017
*/
#include "Rifle.h"

/**
 * Date: Feb 8, 2017
 * Modified: Mar 13, 2017 - Mark Tattrie
 * Author: Maitiu Morton
 * Function Interface: Rifle::Rifle(int32_t id) : InstantWeapon(RifleVars::TYPE, RifleVars::FIRESOUND,
 *       RifleVars::HITSOUND, RifleVars::RELOADSOUND, RifleVars::EMPTYSOUND, RifleVars::RANGE,
 *       RifleVars::DAMAGE, RifleVars::AOE, RifleVars::PENETRATION, RifleVars::CLIP, RifleVars::CLIPMAX,
 *       RifleVars::AMMO, RifleVars::RELOADDELAY, RifleVars::FIREDELAY, id
 * Description:
 * Ctor for Rifle
 */

Rifle::Rifle(const int32_t id) : InstantWeapon(RifleVars::TYPE, TEXTURES::RIFLE, RifleVars::FIRESOUND, RifleVars::HITSOUND, RifleVars::RELOADSOUND,
        RifleVars::EMPTYSOUND, RifleVars::RANGE, RifleVars::DAMAGE, RifleVars::AOE, RifleVars::PENETRATION, RifleVars::ACCURACY,
        RifleVars::CLIP, RifleVars::CLIPMAX, RifleVars::AMMO, RifleVars::RELOADDELAY, RifleVars::FIREDELAY, id) {

}
