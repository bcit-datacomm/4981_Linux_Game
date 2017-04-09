/*
    Created by Maitiu Morton 2/8/2017
        Edited by DericM 3/8/2017
        Edited by Mark Tattrie 3/13/2017
*/
#include "ShotGun.h"
#include "../../basic/Movable.h"
#include "../../player/Marine.h"
#include "../../log/log.h"

/**
 * Date: Feb 8, 2017
 * Modified: Mar 13, 2017 - Mark Tattrie
 * Author: Maitiu Morton
 * Function Interface: ShotGun::ShotGun(int32_t id) : InstantWeapon(ShotgunVars::TYPE, ShotgunVars::FIRESOUND,
 *       ShotgunVars::HITSOUND, ShotgunVars::RELOADSOUND, ShotgunVars::EMPTYSOUND,
 *       ShotgunVars::RANGE, ShotgunVars::DAMAGE, ShotgunVars::AOE, ShotgunVars::PENETRATION,
 *       ShotgunVars::CLIP, ShotgunVars::CLIPMAX, ShotgunVars::AMMO, ShotgunVars::RELOADDELAY,
 *       ShotgunVars::FIREDELAY, id)
 * Description:
 * Ctor for Shotgun
 */
ShotGun::ShotGun(const int32_t id) : InstantWeapon(ShotgunVars::TYPE, TEXTURES::SHOTGUN, ShotgunVars::FIRESOUND,
        ShotgunVars::HITSOUND, ShotgunVars::RELOADSOUND, ShotgunVars::EMPTYSOUND, ShotgunVars::RANGE,
        ShotgunVars::DAMAGE, ShotgunVars::AOE, ShotgunVars::PENETRATION, ShotgunVars::ACCURACY, ShotgunVars::CLIP,
        ShotgunVars::CLIPMAX, ShotgunVars::AMMO, ShotgunVars::RELOADDELAY, ShotgunVars::FIREDELAY, id) {

}


/**
    ShotGun::fire

    DISCRIPTION:
        Modified version of instant fire. THis will fire multiple projectiles in a cone infront
        of the movable.

        Movable& movable: The thing thats holding the weapon that is firing.
        Its needed for its x and y cords, and for its angle.

    AUTHOR: Deric Mccadden 01/03/17

*/
bool ShotGun::fire(Movable& movable){
    if (!Weapon::fire(movable)) {
        return false;
    }
    logv(3, "ShotGun::fire()\n");

    const int gunX = movable.getX() + (MARINE_WIDTH / 2);
    const int gunY = movable.getY() + (MARINE_HEIGHT / 2);
    const double angle = movable.getAngle();

    //cone angle is how far the buckshot will spread out.
    const double coneAngle = ShotgunVars::SPREAD_ANGLE;
    //number of shots is how many buckshot pellets we are firing.
    const double numberOfShots = ShotgunVars::SPREAD_SHOTS;
    //seperation between buckshot pellets.
    const double seperation = coneAngle / numberOfShots;

    const double startAngle = angle - (coneAngle / 2);

    double currentAngle = startAngle;
    for(int i = 0; i < numberOfShots; i++){
        fireSingleProjectile(gunX, gunY, currentAngle);
        currentAngle += seperation;
    }

    return true;
}
