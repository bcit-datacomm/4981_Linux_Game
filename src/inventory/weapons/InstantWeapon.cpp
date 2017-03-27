/**
    InstantWeapon.cpp

    DISCRIPTION:
        InstantWewapon Weapons construct a line from the weapons mussle to its
        theoretical range limit, and then get all the intersecting targets.
        Tergets are sorted in a priority queue by distance from player, and then
        they are damaged in order untill something invulnrable is hit, or
        penertation runs out.

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
#include "../../sprites/VisualEffect.h"

using std::string;

InstantWeapon::InstantWeapon(string type, string fireSound, string hitSound, string reloadSound, string emptySound,
        int range, int damage, int AOE, int penetration, int clip, int clipMax, int ammo, int reloadDelay, int fireDelay)
: Weapon(type, fireSound, hitSound, reloadSound, emptySound, range, damage, AOE, penetration, clip, clipMax, ammo,
        reloadDelay, fireDelay) {

}


// DericM, 01/03/17
bool InstantWeapon::fire(Marine &marine) {

    if(!Weapon::fire(marine)) {
        return false;
    }
    logv("InstantWeapon::fire()\n");

    std::priority_queue<Target> targets;
    GameManager* gameManager = GameManager::instance();
    auto& collisionHandler = gameManager->getCollisionHandler();

    collisionHandler.detectLineCollision(targets, marine, range);

    for(int i = 0; i < penetration; i++) {
        if(targets.empty()) {
            logv("targets.empty()\n");
            break;
        }
        if(targets.top().getType() != TYPE_ZOMBIE) {
            logv("target is of type: %d\n", targets.top().getType());
            break;
        }

        logv("targets.size():%d\n", targets.size());
        logv("Shot target of type: %d\n", targets.top().getType());

        int32_t id = targets.top().getId();
        if(!gameManager->zombieExists(id)) {
            logv("!gameManager.zombieExists(id)\n");
            break;
        }
        gameManager->getZombie(id).collidingProjectile(damage);
        targets.pop();
    }


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
    return true;
}
