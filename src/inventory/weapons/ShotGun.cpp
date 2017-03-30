/*
    Created by Maitiu Morton 2/8/2017
        Edited by DericM 3/8/2017
        Edited by Mark Tattrie 3/13/2017
*/
#include "ShotGun.h"

/**
 * Date: Feb 8, 2017
 * Modified: Mar 13, 2017 - Mark Tattrie
 * Author: Maitiu Morton
 * Function Interface: ShotGun::ShotGun() : InstantWeapon(ShotgunVars::TYPE, ShotgunVars::FIRESOUND,
 *       ShotgunVars::HITSOUND, ShotgunVars::RELOADSOUND, ShotgunVars::EMPTYSOUND,
 *       ShotgunVars::RANGE, ShotgunVars::DAMAGE, ShotgunVars::AOE, ShotgunVars::PENETRATION,
 *       ShotgunVars::CLIP, ShotgunVars::CLIPMAX, ShotgunVars::AMMO, ShotgunVars::RELOADDELAY,
 *       ShotgunVars::FIREDELAY)
 * Description:
 * Ctor for Shotgun
 */
ShotGun::ShotGun() : InstantWeapon(ShotgunVars::TYPE, ShotgunVars::FIRESOUND, ShotgunVars::HITSOUND,
        ShotgunVars::RELOADSOUND, ShotgunVars::EMPTYSOUND, ShotgunVars::RANGE, ShotgunVars::DAMAGE,
        ShotgunVars::AOE, ShotgunVars::PENETRATION, ShotgunVars::CLIP, ShotgunVars::CLIPMAX, ShotgunVars::AMMO,
        ShotgunVars::RELOADDELAY, ShotgunVars::FIREDELAY) {
}
