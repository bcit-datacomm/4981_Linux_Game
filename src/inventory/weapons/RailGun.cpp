/*
    Created by Deric Mccadden Apr/6/2017
*/
#include "RailGun.h"

/**
 * Date: Feb Apr/6/2017
 * Author: Deric Mccadden
 * RailGun::RailGun(const int32_t id) : InstantWeapon(RifleVars::TYPE, TEXTURES::RIFLE, RifleVars::FIRESOUND, 
 		RifleVars::HITSOUND, RifleVars::RELOADSOUND, RifleVars::EMPTYSOUND, RifleVars::RANGE, RifleVars::DAMAGE, 
 		RifleVars::AOE, RifleVars::PENETRATION, RifleVars::ACCURACY, RifleVars::CLIP, RifleVars::CLIPMAX, 
 		RifleVars::AMMO, RifleVars::RELOADDELAY, RifleVars::FIREDELAY, id) 
 * Ctor for RailGun
 */

RailGun::RailGun(const int32_t id) : InstantWeapon(RailGunVars::TYPE, TEXTURES::RIFLE, RailGunVars::FIRESOUND, 
		RailGunVars::HITSOUND, RailGunVars::RELOADSOUND, RailGunVars::EMPTYSOUND, RailGunVars::RANGE, RailGunVars::DAMAGE, 
		RailGunVars::AOE, RailGunVars::PENETRATION, RailGunVars::ACCURACY, RailGunVars::CLIP, RailGunVars::CLIPMAX, 
		RailGunVars::AMMO, RailGunVars::RELOADDELAY, RailGunVars::FIREDELAY, RailGunVars::START_X, RailGunVars::START_Y, id) {

}
