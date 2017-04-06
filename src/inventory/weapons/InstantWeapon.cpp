/**
    InstantWeapon.cpp

    DISCRIPTION:
        InstantWewapon Weapons construct a line from the weapons mussle to its
        range limit, and then gets all the intersecting targets.
        Tergets are sorted in a priority queue by distance from player, and then
        they are damaged in order untill something invulnrable is hit, or
        penertation runs out.

    AUTHOR: Deric Mccadden 01/03/17

*/
#include <queue>
#include <cstdio>
#include <iostream>

#include "InstantWeapon.h"
#include "../../collision/HitBox.h"
#include "../../game/GameManager.h"
#include "../../collision/CollisionHandler.h"
#include "../../audio/AudioManager.h"
#include "../../log/log.h"
#include "Target.h"
#include "../../sprites/VisualEffect.h"

using std::string;

/**
 * Date: Mar 1, 2017
 * Modified: Mar 13, 2017 - Mark Tattrie
 * Author: Deric Mccadden
 * Function Interface: InstantWeapon::InstantWeapon(string type, string fireSound, string hitSound,
 *       string reloadSound, string emptySound, int range, int damage, int AOE, int penetration,
 *       int clip, int clipMax, int ammo, int reloadDelay, int fireDelay, int32_t id)
 *       : Weapon(type, fireSound, hitSound, reloadSound, emptySound, range, damage, AOE,
 *          penetration, clip, clipMax, ammo, reloadDelay, fireDelay, id)
 * Description:
 * Ctor for Instant Weapon
 */
InstantWeapon::InstantWeapon(string type, TEXTURES sprite, string fireSound, string hitSound,
        string reloadSound, string emptySound, int range, int damage, int AOE, int penetration,
        int clip, int clipMax, int ammo, int reloadDelay, int fireDelay, int32_t id)
: Weapon(type, sprite, fireSound, hitSound, reloadSound, emptySound, range, damage, AOE,
        penetration, clip, clipMax, ammo, reloadDelay, fireDelay, id) {

}


/**
    InstantWeapon::fire

    DISCRIPTION:
        Default behaviour for instant weapon is that it fires one projectile in the direction
        that the movable is facing.

        Movable& movable: The thing thats holding the weapon that is firing.
        Its needed for its x and y cords, and for its angle.

    AUTHOR: Deric Mccadden 01/03/17

*/
bool InstantWeapon::fire(Movable& movable) {
    if (!Weapon::fire(movable)) {
        return false;
    }
    logv(3, "InstantWeapon::fire()\n");

    const int gunX = movable.getX() + (MARINE_WIDTH / 2);
    const int gunY = movable.getY() + (MARINE_HEIGHT / 2);
    const double angle = movable.getAngle();

    fireSingleProjectile(gunX, gunY, angle);

    return true;
}



/**
    InstantWeapon::fireSingleProjectile

    DISCRIPTION:
        construct a line from the weapons mussle to its
        range limit, and then gets all the intersecting targets.
        Tergets are sorted in a priority queue by distance from player, and then
        they are damaged in order untill something invulnrable is hit, or
        penertation runs out.

        int gunX, int gunY
            The x and y coordinates of the guns muzzle.

        double angle
            the angle gun is facing.

    AUTHOR: Deric Mccadden 01/03/17

*/
void InstantWeapon::fireSingleProjectile(const int gunX, const int gunY, const double angle){
    TargetList targetList;

    GameManager::instance()->getCollisionHandler().detectLineCollision(targetList, gunX, gunY, angle, range);

    int finalX = targetList.getEndX();
    int finalY = targetList.getEndY();

    for(int i = 0; i <= penetration; i++) {
        if (targetList.isEmpty()) {
            logv(3, "targets.empty()\n");
            break;
        }
        Target target = targetList.getNextTarget();

        //if we have run out of penatration set the end point to here.
        if(i == penetration){
            finalX = target.getHitX();
            finalY = target.getHitY();
        }

        //if the target is invincible break because we cant hit anything more.
        if (!target.isType(TYPE_ZOMBIE)) {
            finalX = target.getHitX();
            finalY = target.getHitY();
            logv(3, "target is of type: %d\n", target.getType());
            break;
        }

        logv(3, "targets.size():%d\n", targetList.numTargets());
        logv(3, "Shot target of type: %d\n", target.getType());

        int32_t id = target.getId();

        if (!GameManager::instance()->zombieExists(id)) {
            logv(3, "!gameManager.zombieExists(id)\n");
            break;
        }
        //damage target
        GameManager::instance()->getZombie(id).collidingProjectile(damage);
        targetList.removeTop();
    }

    VisualEffect::instance().addPreLine(2, targetList.getOriginX(), targetList.getOriginY(), finalX, finalY, 25, 70, 193);
}
