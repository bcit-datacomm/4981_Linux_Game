/*
    Created  by Deric M       01/03/17
*/
#include "InstantWeapon.h"
#include "../HitBox.h"
#include "../GameManager.h"
#include "../CollisionHandler.h"
#include "../AudioManager.h"
#include <queue>
#include <stdio.h>
#include <iostream>

InstantWeapon::InstantWeapon(std::string type, int range, int damage, 
        int clip, int clipMax, int ammo, int AOE, int reloadSpeed)
: Weapon(type, range, damage, clip, clipMax, ammo, AOE, reloadSpeed) {

}



// DericM, 01/03/17
void InstantWeapon::fire(Marine &marine){
	//check ammo    

    printf("InstantWeapon::fire()\n");

    CollisionHandler collisionHandler = GameManager::instance()->getCollisionHandler();

    if(!reduceAmmo(1)){
    	return;
    }

    //AudioManager::instance().playEffect(EFX_WLPISTOL);

    //get all targets in line with the shot
    std::priority_queue<HitBox*> targets;
    targets = collisionHandler.detectLineCollision(marine, getRange());
    if(targets.empty()){
    	return;
    }

    //=======================================================
    //only shoot the first target for now, will change this later
    //to include penetration to shoot multiple targets with 1 bullet.
    //targets.top()->attached->collidingProjectile(getDamage()); //broken
    targets.pop();
    //=======================================================
    
}
