#include "Quadtree.h"
#include "CollisionHandler.h"
#include "../player/Marine.h"
#include "../log/log.h"
#include <iostream>
#include <cmath>
#include <cassert>
#include "../inventory/weapons/Target.h"


CollisionHandler::CollisionHandler() : quadtreeMarine(0, {0,0,2000,2000}), quadtreeZombie(0, {0,0,2000,2000}),
        quadtreeBarricade(0, {0,0,2000,2000}),quadtreeTurret(0, {0,0,2000,2000}),
        quadtreeWall(0, {0,0,2000,2000}), quadtreePickUp(0, {0,0,2000,2000}), quadtreeObj(0, {0,0,2000,2000}) {

}

CollisionHandler& CollisionHandler::operator=(const CollisionHandler& handle) {
    quadtreeMarine = handle.quadtreeMarine;
    quadtreeZombie = handle.quadtreeZombie;
    quadtreeBarricade = handle.quadtreeBarricade;
    quadtreeTurret = handle.quadtreeTurret;
    quadtreeWall = handle.quadtreeWall;
    quadtreePickUp = handle.quadtreePickUp;
    quadtreeObj = handle.quadtreeObj;
    return *this;
}

// Check for projectile collisions, return object it hits
const HitBox *CollisionHandler::detectDamageCollision(std::vector<Entity*> returnObjects, const Entity *entity) {
    for (const auto& obj: returnObjects){
        if (obj != nullptr && entity != obj
            && SDL_HasIntersection(&entity->getDamHitBox().getRect(), &obj->getDamHitBox().getRect())
                && !(entity->getDamHitBox().isPlayerFriendly() && obj->getDamHitBox().isPlayerFriendly())){
            return &(obj->getDamHitBox());
        }
    }
    return nullptr;
}

// Check for projectile collisions, return object it hits
const HitBox *CollisionHandler::detectProjectileCollision(std::vector<Entity*> returnObjects, const Entity *entity) {
    for (const auto& obj: returnObjects){
        if (obj != nullptr && entity != obj
            && SDL_HasIntersection(&entity->getProHitBox().getRect(), &obj->getProHitBox().getRect())
                && !(entity->getProHitBox().isPlayerFriendly() && obj->getProHitBox().isPlayerFriendly())){
            return &(obj->getProHitBox());
        }
    }
    return nullptr;
}

// Check for collisions during movement
bool CollisionHandler::detectMovementCollision(std::vector<Entity*> returnObjects, const Entity *entity) {
    for (const auto& obj: returnObjects){
        if (obj != nullptr && entity != obj
            && SDL_HasIntersection(&entity->getMoveHitBox().getRect(), &obj->getMoveHitBox().getRect())
                && !(entity->getMoveHitBox().isPlayerFriendly() && obj->getMoveHitBox().isPlayerFriendly())){
            return true;
        }
    }
    return false;
}

//check for pickup collision
Entity *CollisionHandler::detectPickUpCollision(std::vector<Entity*> returnObjects, const Entity *entity) {
    for (const auto& obj: returnObjects){
        if (obj != nullptr && entity != obj
            && SDL_HasIntersection(&entity->getMoveHitBox().getRect(), &obj->getPickUpHitBox().getRect())
                && !(entity->getMoveHitBox().isPlayerFriendly() && obj->getPickUpHitBox().isPlayerFriendly())){
            return obj;
        }
    }
    logv("nothing to pick up\n");
    return nullptr;

}

// Created by DericM 3/8/2017
//      fixed to use new quad trees (cant shoot walls yet)
std::priority_queue<Target> CollisionHandler::detectLineCollision(
        Marine &marine, const int range) {

    const double degrees = marine.getAngle() - 90;
    const double radians = degrees * M_PI / 180;
    const int playerX = marine.getX() + (MARINE_WIDTH / 2);
    const int playerY = marine.getY() + (MARINE_HEIGHT / 2);
    const int deltaX  = range * cos(radians);
    const int deltaY  = range * sin(radians);
    int aX; 
    int aY;
    int bX;
    int bY; 
    int tX; 
    int tY; 
    int pDist;

    std::vector<Entity*> allEntities;
    std::priority_queue<Target> targetsInSights;

    auto& zombies = quadtreeZombie.objects;
    auto& turrets = quadtreeTurret.objects;
    auto& walls   = quadtreeWall.objects;

    allEntities.reserve(zombies.size() + turrets.size() + walls.size());
    allEntities.insert(allEntities.end(), zombies.begin(), zombies.end());
    allEntities.insert(allEntities.end(), turrets.begin(), turrets.end());
    allEntities.insert(allEntities.end(), walls.begin(), walls.end());

    for (unsigned int x = 0, len = allEntities.size(); x < len; x++) {
        Entity* possibleTarget = allEntities.at(x);

        aX = playerX;
        aY = playerY;
        bX = aX + deltaX;
        bY = aY + deltaY;

        if (SDL_IntersectRectAndLine(&(possibleTarget->getProHitBox().getRect()), &aX, &aY , &bX, &bY)) {
            logi("Intersect target at (%d, %d)\n", bX, bY);

            tX = marine.getX() - bX;
            tY = marine.getY() - bX;
            pDist = std::hypot(tX, tY);

            Target tar(*possibleTarget);
            tar.hitX = aX;
            tar.hitY = aY;
            tar.playerDist = pDist;

            targetsInSights.push(tar);
            logi("targetsInSights.size(): %d\n", targetsInSights.size());
            
        }
    }
    return targetsInSights;
}


std::vector<Entity *> CollisionHandler::getQuadTreeEntities(Quadtree &q, const Entity *entity){
    std::vector<Entity*> returnObjects;
    return q.retrieve(entity);
}

