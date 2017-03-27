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
#include "../../sprites/VisualEffect.h"

InstantWeapon::InstantWeapon(std::string type, int range, int damage,
        int clip, int clipMax, int ammo, int AOE, int reloadSpeed, int fireRate, bool isReadyToFire)
        : Weapon(type, range, damage, clip, clipMax, ammo, AOE, reloadSpeed, fireRate, isReadyToFire) {

}



// DericM, 01/03/17
void InstantWeapon::fire(Marine &marine){
 //check ammo

    logv("InstantWeapon::fire()\n");

    CollisionHandler &collisionHandler = GameManager::instance()->getCollisionHandler();

    if(!reduceAmmo(1)){
        return;
    }

    //AudioManager::instance().playEffect(EFX_WLPISTOL);

    //get all targets in line with the shot
    std::priority_queue<const HitBox*> targets;
    targets = collisionHandler.detectLineCollision(marine, getRange());
   

    //similar as to what happens in the line collision and is used to show a use of the line effect
    const double degrees = marine.getAngle() - 90;
    const double radians = degrees * M_PI / 180;
    const int playerX = marine.getX() + (MARINE_WIDTH / 2);
    const int playerY = marine.getY() + (MARINE_HEIGHT / 2);
    const int deltaX  = playerX + range * cos(radians);
    const int deltaY  = playerY + range * sin(radians);

    //5 frames to display ie 1/12th of a second at 60fps
    //0 red, 255 green, 0 blue
    VisualEffect::instance().addPreLine(5, playerX, playerY, deltaX, deltaY, 0, 255, 0);

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
