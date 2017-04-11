/*
    Created by Deric Mccadden Apr/6/2017
*/
#include "PhaseRepeater.h"

/**
 * Date: Feb Apr/6/2017
 * Author: Deric Mccadden
 * RailGun::RailGun(const int32_t id) : InstantWeapon(PhaseRepeaterVars::TYPE, TEXTURES::RIFLE, 
 		PhaseRepeaterVars::FIRESOUND, PhaseRepeaterVars::HITSOUND, PhaseRepeaterVars::RELOADSOUND,
        PhaseRepeaterVars::EMPTYSOUND, PhaseRepeaterVars::RANGE, PhaseRepeaterVars::DAMAGE, PhaseRepeaterVars::AOE, 
        PhaseRepeaterVars::PENETRATION, PhaseRepeaterVars::ACCURACY, PhaseRepeaterVars::CLIP, PhaseRepeaterVars::CLIPMAX, 
        PhaseRepeaterVars::AMMO, PhaseRepeaterVars::RELOADDELAY, PhaseRepeaterVars::FIREDELAY, id) 
 * Ctor for RailGun
 */

PhaseRepeater::PhaseRepeater(const int32_t id) : InstantWeapon(PhaseRepeaterVars::TYPE, TEXTURES::RIFLE, 
		PhaseRepeaterVars::FIRESOUND, PhaseRepeaterVars::HITSOUND, PhaseRepeaterVars::RELOADSOUND,
        PhaseRepeaterVars::EMPTYSOUND, PhaseRepeaterVars::RANGE, PhaseRepeaterVars::DAMAGE, PhaseRepeaterVars::AOE, 
        PhaseRepeaterVars::PENETRATION, PhaseRepeaterVars::ACCURACY, PhaseRepeaterVars::CLIP, PhaseRepeaterVars::CLIPMAX, 
        PhaseRepeaterVars::AMMO, PhaseRepeaterVars::RELOADDELAY, PhaseRepeaterVars::FIREDELAY, PhaseRepeaterVars::START_X,
        PhaseRepeaterVars::START_Y, id) {

}
