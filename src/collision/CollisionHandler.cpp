#include "Quadtree.h"
#include "CollisionHandler.h"
#include "../player/Marine.h"
#include "../log/log.h"
#include <iostream>

CollisionHandler::CollisionHandler() : quadtreeMov(0, {0,0,2000,2000}), quadtreePro(0, {0,0,2000,2000}),
        quadtreeDam(0, {0,0,2000,2000}),quadtreePickUp(0, {0,0,2000,2000}) {

}

CollisionHandler& CollisionHandler::operator=(const CollisionHandler& handle) {
    quadtreeMov = handle.quadtreeMov;
    quadtreePro = handle.quadtreePro;
    quadtreeDam = handle.quadtreeDam;
    quadtreePickUp = handle.quadtreePickUp;
    return *this;
}

// Check for projectile collisions, return object it hits
HitBox *CollisionHandler::detectDamageCollision(const Entity *entity) {
    std::vector<Entity*> returnObjects;
    returnObjects = quadtreeDam.retrieve(entity);

    for (unsigned int x = 0, len = returnObjects.size(); x < len; x++) {
        if (returnObjects[x] != nullptr && entity != returnObjects[x]
            && SDL_HasIntersection(&entity->damageHitBox->getRect(), &returnObjects[x]->damageHitBox->getRect())
                && !(entity->damageHitBox->isPlayerFriendly() && returnObjects[x]->damageHitBox->isPlayerFriendly()) ) {
            return returnObjects[x]->damageHitBox.get();
        }
    }
    return nullptr;
}

// Check for projectile collisions, return object it hits
HitBox *CollisionHandler::detectProjectileCollision(const Entity *entity) {
    std::vector<Entity*> returnObjects;
    returnObjects = quadtreePro.retrieve(entity);
    for (unsigned int x = 0, len = returnObjects.size(); x < len; x++) {
        if (returnObjects[x] != nullptr && entity != returnObjects[x]
            && SDL_HasIntersection(&entity->projectileHitBox->getRect(), &returnObjects[x]->projectileHitBox->getRect())
                && !(entity->projectileHitBox->isPlayerFriendly() && returnObjects[x]->projectileHitBox->isPlayerFriendly()) ) {
            return returnObjects[x]->projectileHitBox.get();
        }
    }
    return nullptr;
}

// Check for collisions during movement
bool CollisionHandler::detectMovementCollision(const Entity *entity) {
    std::vector<Entity*> returnObjects;
    returnObjects = quadtreeMov.retrieve(entity);
    for (unsigned int x = 0, len = returnObjects.size(); x < len; x++) {
        if (returnObjects[x] != nullptr && entity != returnObjects[x]
            && SDL_HasIntersection(&entity->movementHitBox->getRect(), &returnObjects[x]->movementHitBox->getRect())
                && !(entity->movementHitBox->isPlayerFriendly() && returnObjects[x]->movementHitBox->isPlayerFriendly()) ) {
            return true;
        }
    }
    return false;
}

//check for pickup collision
Entity *CollisionHandler::detectPickUpCollision(const Entity *entity) {

    std::vector<Entity*> returnObjects;
    returnObjects = quadtreePickUp.retrieve(entity);

    for (unsigned int x = 0, len = returnObjects.size(); x < len; x++) {
        if (returnObjects[x] != nullptr && entity != returnObjects[x]
            && SDL_HasIntersection(&entity->movementHitBox->getRect(), &returnObjects[x]->pickupHitBox->getRect())
                && !(entity->movementHitBox->isPlayerFriendly() && returnObjects[x]->pickupHitBox->isPlayerFriendly()) ) {
            return returnObjects[x];
        }
    }
    return nullptr;
}

// Created by DericM 3/8/2017
std::priority_queue<HitBox*> CollisionHandler::detectLineCollision(
        Marine &marine, const int range) {

    const double degrees = marine.getAngle() - 90;
    const double radians = degrees * M_PI / 180;
    const int playerX = marine.getX() + (MARINE_WIDTH / 2);
    const int playerY = marine.getY() + (MARINE_HEIGHT / 2);
    const int deltaX  = range * cos(radians);
    const int deltaY  = range * sin(radians);
    int aX, aY, bX, bY;

    std::vector<Entity*> allEntities;
    allEntities = quadtreePro.objects;

    std::priority_queue<HitBox*> targetsInSights;
    for (unsigned int x = 0, len = allEntities.size(); x < len; x++) {
            aX = playerX;
            aY = playerY;
            bX = aX + deltaX;
            bY = aY + deltaY;

            if (SDL_IntersectRectAndLine(&(allEntities.at(x)->projectileHitBox->getRect()), &aX, &aY , &bX, &bY)) {
                logv("Shot target at (%d, %d)\n", aX, aY);
                targetsInSights.push(allEntities.at(x)->projectileHitBox.get());
            }
    }
    return targetsInSights;
}
