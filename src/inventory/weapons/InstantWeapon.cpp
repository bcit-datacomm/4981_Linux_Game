/**
    Target.h

    DISCRIPTION:
        This is class is used in all instant fire weapons, the weapons will 

    AUTHOR: Deric Mccadden 01/03/17

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
    int range, int damage, int AOE, int penetration, int clip, int clipMax, int ammo, int reloadDelay, int fireDelay)
: Weapon(type, fireSound, hitSound, reloadSound, emptySound, range, damage, AOE, penetration, clip, clipMax, ammo, 
    reloadDelay, fireDelay) {

}


// DericM, 01/03/17
bool InstantWeapon::fire(Marine &marine){

    if(!Weapon::fire(marine)){
        return false;
    }
    logi("InstantWeapon::fire()\n");

    auto& collisionHandler = GameManager::instance()->getCollisionHandler();

    std::priority_queue<Target> targets;

    collisionHandler.detectLineCollision(targets, marine, range);//getTargets(marine);


    for(int i = 0; i < penetration; i++) {
        if(targets.empty()){
            logi("targets.empty()\n");
            break;
        }
        if(targets.top().getType() != TYPE_ZOMBIE){
            logi("target is of type: %d\n", targets.top().getType());
            break;
        }

        logi("targets.size():%d\n", targets.size());

        logi("Shot target of type: %d\n", targets.top().getType());
        targets.top().getEntity().collidingProjectile(damage);
        targets.pop();
    }

    return true;
}

