/**
 * Date: Mar. 30, 2017
 * Designer: Mark Chen, Maitiu Morton, Deric Mccadden
 * Programmer: Mark Chen
 * Functions: TurretGun()
 *
 * Description:
 * Source file for the TurretGun class. This file contains the TurretGun constructor.
 */
#include "TurretGun.h"

TurretGun::TurretGun(int32_t id) : InstantWeapon(TurretGunVars::TYPE, TEXTURES::SHOTGUN, TurretGunVars::FIRESOUND, TurretGunVars::HITSOUND,
        TurretGunVars::RELOADSOUND, TurretGunVars::EMPTYSOUND, TurretGunVars::RANGE, TurretGunVars::DAMAGE,
        TurretGunVars::AOE, TurretGunVars::PENETRATION, TurretGunVars::ACCURACY, TurretGunVars::CLIP, TurretGunVars::CLIPMAX, TurretGunVars::AMMO,
        TurretGunVars::RELOADDELAY, TurretGunVars::FIREDELAY, id) {

}
