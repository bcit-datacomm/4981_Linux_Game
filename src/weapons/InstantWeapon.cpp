/*
    Created  by Deric M       01/03/17
*/
#include "InstantWeapon.h"
#include "../HitBox.h"
#include "../GameManager.h"
#include "../CollisionHandler.h"
#include <queue>
#include <stdio.h>
#include <iostream>

InstantWeapon::InstantWeapon(std::string type, int range, int damage,
        int clip, int clipMax, int ammo, int AOE, int reloadSpeed, int fireRate, bool isReadyToFire)
: Weapon(type, range, damage, clip, clipMax, ammo, AOE, reloadSpeed, fireRate, isReadyToFire) {

}



// DericM, 01/03/17
void InstantWeapon::fire(Marine &marine){
 //check ammo

    printf("InstantWeapon::fire()\n");

    CollisionHandler collisionHandler = GameManager::instance()->getCollisionHandler();

    if(!reduceAmmo(1)){
     return;
    }

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

void InstantWeapon::melee(Marine &marine){
    printf("InstantWeapon::melee\n");

    CollisionHandler ch = GameManager::instance()->getCollisionHandler();

    int x = marine.getX();
    int y = marine.getY();
    SDL_Rect checkBox;
    checkBox.h = 100;
    checkBox.w = 100;
    checkBox.x = x;
    checkBox.y = y;
    HitBox hitBox(x, y, checkBox, nullptr);

    if(ch.detectMovementCollision(&hitBox)){
        printf("melee hit");
    }


}
