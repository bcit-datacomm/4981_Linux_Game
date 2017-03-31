/**
    Created  by Mark Tattrie    March 28, 2017
*/
#include "MeleeWeapon.h"
#include "../../collision/HitBox.h"
#include "../../game/GameManager.h"
#include "../../collision/CollisionHandler.h"
#include "../../audio/AudioManager.h"
#include <queue>
#include <stdio.h>
#include <iostream>
#include "../../log/log.h"
#include <memory>

/**
 * Date: Mar 28, 2017
 * Author: Mark Tattrie
 * Function Interface: MeleeWeapon::MeleeWeapon(string type, TEXTURES sprite, string fireSound,
 *       string hitSound, string reloadSound, string emptySound, int range, int damage, int AOE,
 *       int penetration, int clip, int clipMax, int ammo, int reloadDelay, int fireDelay, int32_t id)
 * Description:
 * Ctor for MeleeWeapon
 */
MeleeWeapon::MeleeWeapon(string type, TEXTURES sprite, string fireSound, string hitSound, string reloadSound,
        string emptySound, int range, int damage, int AOE, int penetration, int clip,
        int clipMax, int ammo, int reloadDelay, int fireDelay, int32_t id)
: Weapon(type, sprite, fireSound, hitSound, reloadSound, emptySound, range, damage, AOE,
        penetration, clip, clipMax, ammo, reloadDelay, fireDelay, id) {

}

/**
 * Date: Mar 28, 2017
 * Author: Mark Tattrie
 * Function Interface: bool MeleeWeapon::fire(Movable &mov)
 * Description:
 * Melee weapon takes in a Movable entity, grabs its position and angle then puts a hitbox infront
 * of it based on the range of the weapon. Then checks for collision between that hitbox and other
 * objects.
 */
bool MeleeWeapon::fire(Movable &mov){
    const double degrees = mov.getAngle() - 90;
    const double radians = degrees * M_PI / 180;

    const int originX = mov.getX() + (Z_WIDTH / 2);
    const int originY = mov.getY() + (Z_HEIGHT / 2);
    const int deltaX  = range/2 * cos(radians);
    const int deltaY  = range/2 * sin(radians);

    const int endX = originX + deltaX;
    const int endY = originY + deltaY;

    const int hitBoxX = endX - (range / 2);
    const int hitBoxY = endY - (range / 2);
    const int hitBoxW = range;
    const int hitBoxH = range;

    SDL_Rect meleeBox = {hitBoxX, hitBoxY, hitBoxW, hitBoxH};
    HitBox hitBox(meleeBox);

    CollisionHandler &ch = GameManager::instance()->getCollisionHandler();

    //vector of each entity a zombie needs to check
    std::vector<Entity *> hitMarines;
    std::vector<Entity *> hitTurrets;
    std::vector<Entity *> hitBarricades;

    hitMarines = ch.detectMeleeCollision(ch.getQuadTreeEntities(ch.quadtreeMarine, &mov),&mov, hitBox);
    for(auto& x: hitMarines){
        x->collidingProjectile(damage);
        logv("hit a marine, marine hp = %d\n", dynamic_cast<Marine *>(x)->getHealth());
    }

    hitTurrets = ch.detectMeleeCollision(ch.getQuadTreeEntities(ch.quadtreeTurret, &mov),&mov, hitBox);
    for(auto& y: hitTurrets){//update each zombie's helth
        y->collidingProjectile(damage);
        logv("hit a turret, turret > 0 = %d\n", dynamic_cast<Turret *>(y)->healthCheckTurret());
    }

    hitBarricades = ch.detectMeleeCollision(ch.getQuadTreeEntities(ch.quadtreeBarricade, &mov),&mov, hitBox);
    for(auto& z: hitTurrets){//update each zombie's helth
        //update hitbox attached entity here
        z->collidingProjectile(damage);
    }

    return true;
}
