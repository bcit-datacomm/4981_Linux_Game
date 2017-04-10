/*
    Created by Deric Mccadden Apr/6/2017
*/
#include "PlasmaDeagle.h"

/**
 * Date: Feb Apr/6/2017
 * Author: Deric Mccadden
 * RailGun::RailGun(const int32_t id) : InstantWeapon(PlasmaDeagleVars::TYPE, TEXTURES::RIFLE, 
 		PlasmaDeagleVars::FIRESOUND, PlasmaDeagleVars::HITSOUND, PlasmaDeagleVars::RELOADSOUND,
        PlasmaDeagleVars::EMPTYSOUND, PlasmaDeagleVars::RANGE, PlasmaDeagleVars::DAMAGE, PlasmaDeagleVars::AOE, 
		PlasmaDeagleVars::PENETRATION, PlasmaDeagleVars::ACCURACY, PlasmaDeagleVars::CLIP, PlasmaDeagleVars::CLIPMAX, 
		PlasmaDeagleVars::AMMO, PlasmaDeagleVars::RELOADDELAY, PlasmaDeagleVars::FIREDELAY, id) 
 * Ctor for RailGun
 */

PlasmaDeagle::PlasmaDeagle(const int32_t id) : InstantWeapon(PlasmaDeagleVars::TYPE, TEXTURES::RIFLE, 
		PlasmaDeagleVars::FIRESOUND, PlasmaDeagleVars::HITSOUND, PlasmaDeagleVars::RELOADSOUND,
        PlasmaDeagleVars::EMPTYSOUND, PlasmaDeagleVars::RANGE, PlasmaDeagleVars::DAMAGE, PlasmaDeagleVars::AOE, 
        PlasmaDeagleVars::PENETRATION, PlasmaDeagleVars::ACCURACY, PlasmaDeagleVars::CLIP, PlasmaDeagleVars::CLIPMAX, 
        PlasmaDeagleVars::AMMO, PlasmaDeagleVars::RELOADDELAY, PlasmaDeagleVars::FIREDELAY, PlasmaDeagleVars::START_X,
        PlasmaDeagleVars::START_Y, id) {

}
