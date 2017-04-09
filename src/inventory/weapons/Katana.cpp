/*
    Created by Deric Mccadden Apr/6/2017
*/
#include "Katana.h"
#include "../../basic/Movable.h"
#include "../../player/Marine.h"
#include "../../log/log.h"

/**
 * Created by Deric Mccadden Apr/6/2017
 * Function Interface: Katana::Katana() : InstantWeapon(KatanaVars::TYPE, KatanaVars::FIRESOUND,
 *       KatanaVars::HITSOUND, KatanaVars::RELOADSOUND, KatanaVars::EMPTYSOUND, KatanaVars::RANGE,
 *       KatanaVars::DAMAGE, KatanaVars::AOE, KatanaVars::PENETRATION, KatanaVars::CLIP,
 *       KatanaVars::CLIPMAX, KatanaVars::AMMO, KatanaVars::RELOADDELAY, KatanaVars::FIREDELAY, id)
 * Description:
 * Ctor for Katana
 */
Katana::Katana(const int32_t id) : InstantWeapon(KatanaVars::TYPE, TEXTURES::SHOTGUN, KatanaVars::FIRESOUND,
        KatanaVars::HITSOUND, KatanaVars::RELOADSOUND, KatanaVars::EMPTYSOUND, KatanaVars::RANGE,
        KatanaVars::DAMAGE, KatanaVars::AOE, KatanaVars::PENETRATION, KatanaVars::ACCURACY, KatanaVars::CLIP,
        KatanaVars::CLIPMAX, KatanaVars::AMMO, KatanaVars::RELOADDELAY, KatanaVars::FIREDELAY, id) {

}



/**
    Katana::fire

    DISCRIPTION:
        Modified version of instant fire. THis will fire multiple projectiles in a cone infront
        of the movable. Identical to shotgun.

        Movable& movable: The thing thats holding the weapon that is firing.
        Its needed for its x and y cords, and for its angle.

    AUTHOR: Deric Mccadden Apr/6/2017

*/
bool Katana::fire(Movable& movable){
    if (!Weapon::fire(movable)) {
        return false;
    }
    logv(3, "Katana::fire()\n");

    const int gunX = movable.getX() + (MARINE_WIDTH / 2);
    const int gunY = movable.getY() + (MARINE_HEIGHT / 2);
    const double angle = movable.getAngle();

    //cone angle is how far the buckshot will spread out.
    const double coneAngle = KatanaVars::SPREAD_ANGLE;
    //number of shots is how many buckshot pellets we are firing.
    const double numberOfShots = KatanaVars::SPREAD_SHOTS;
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