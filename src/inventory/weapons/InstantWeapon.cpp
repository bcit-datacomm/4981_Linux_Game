/*
    Created  by Deric M       01/03/17
*/
#include "InstantWeapon.h"
#include "../../collision/HitBox.h"
#include "../../game/GameManager.h"
#include "../../collision/CollisionHandler.h"
#include "../../audio/AudioManager.h"
#include <queue>
#include <stdio.h>
#include <iostream>
#include "../../log/log.h"
#include "Target.h"

using std::string;





InstantWeapon::InstantWeapon(string type, string fireSound, string hitSound, string reloadSound, string emptySound,
    int range, int damage, int AOE, int clip, int clipMax, int ammo, int reloadDelay, int fireDelay)
: Weapon(type, fireSound, hitSound, reloadSound, emptySound, range, damage, AOE, clip, clipMax, ammo, 
    reloadDelay, fireDelay) {

}


// DericM, 01/03/17
bool InstantWeapon::fire(Marine &marine){

    if(!Weapon::fire(marine)){
        return false;
    }
    logi("InstantWeapon::fire()\n");


    std::priority_queue<Target> targets = getTargets(marine);

    if(targets.empty()){
        return false;
    }


    logi("target hit\n");
    //=======================================================
    //only shoot the first target for now, will change this later
    //to include penetration to shoot multiple targets with 1 bullet.
    //targets.top()->attached->collidingProjectile(getDamage()); //broken
    targets.pop();
    //=======================================================

    return true;
}



std::priority_queue<Target> InstantWeapon::getTargets(Marine &marine){
    CollisionHandler &collisionHandler = GameManager::instance()->getCollisionHandler();

    return collisionHandler.detectLineCollision(marine, getRange());
}
