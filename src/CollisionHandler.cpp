#include "Quadtree.h"
#include "CollisionHandler.h"
#include "Marine.h"
#include "Entity.h"
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
const HitBox *CollisionHandler::detectDamageCollision(const Entity *entity) {
    std::vector<Entity*> returnObjects;
    returnObjects = quadtreeDam.retrieve(entity);
    for (unsigned int x = 0; x < returnObjects.size(); x++) {
        if (returnObjects.at(x) != nullptr && entity != returnObjects.at(x)
            && SDL_HasIntersection(&entity->getDamHitBox().getRect(), &returnObjects.at(x)->getDamHitBox().getRect())
                && !(entity->getDamHitBox().isPlayerFriendly() && returnObjects.at(x)->getDamHitBox().isPlayerFriendly()) ) {
            return &(returnObjects.at(x)->getDamHitBox());
        }
    }
    return nullptr;
}

// Check for projectile collisions, return object it hits
const HitBox *CollisionHandler::detectProjectileCollision(const Entity *entity) {
    std::vector<Entity*> returnObjects;
    returnObjects = quadtreePro.retrieve(entity);
    for (unsigned int x = 0; x < returnObjects.size(); x++) {
        if (returnObjects.at(x) != nullptr && entity != returnObjects.at(x)
            && SDL_HasIntersection(&entity->getProHitBox().getRect(), &returnObjects.at(x)->getProHitBox().getRect())
                && !(entity->getProHitBox().isPlayerFriendly() && returnObjects.at(x)->getProHitBox().isPlayerFriendly()) ) {
            return &(returnObjects.at(x)->getProHitBox());
        }
    }
    return nullptr;
}

// Check for collisions during movement
bool CollisionHandler::detectMovementCollision(const Entity *entity) {
    std::vector<Entity*> returnObjects;
    returnObjects = quadtreeMov.retrieve(entity);
    for (unsigned int x = 0; x < returnObjects.size(); x++) {
        if (returnObjects.at(x) != nullptr && entity != returnObjects.at(x)
            && SDL_HasIntersection(&entity->getMoveHitBox().getRect(), &returnObjects.at(x)->getMoveHitBox().getRect())
                && !(entity->getMoveHitBox().isPlayerFriendly() && returnObjects.at(x)->getMoveHitBox().isPlayerFriendly()) ) {
            return true;
        }
    }
    return false;
}

//check for pickup collision
Entity* CollisionHandler::detectPickUpCollision(const Entity *entity) {

    std::vector<Entity*> returnObjects;
    returnObjects = quadtreePickUp.retrieve(entity);

    for (unsigned int x = 0; x < returnObjects.size(); x++) {
        if (returnObjects.at(x) != nullptr && entity != returnObjects.at(x)
            && SDL_HasIntersection(&entity->getMoveHitBox().getRect(), &returnObjects.at(x)->getPickUpHitBox().getRect())
                && !(entity->getMoveHitBox().isPlayerFriendly() && returnObjects.at(x)->getPickUpHitBox().isPlayerFriendly()) ) {
            return returnObjects.at(x);
        }
    }
    return nullptr;
}

// Created by DericM 3/8/2017
std::priority_queue<const HitBox*> CollisionHandler::detectLineCollision(
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

    std::priority_queue<const HitBox*> targetsInSights;
    for (unsigned int x = 0, len = allEntities.size(); x < len; x++) {
        //if (allEntities.at(x) != marine) {
        //printf("Entity: %d x:%d y:%d\n", x, allEntities.at(x)->projectileHitBox->getRect().x, allEntities.at(x)->projectileHitBox->getRect().y);
            aX = playerX;
            aY = playerY;
            bX = aX + deltaX;
            bY = aY + deltaY;

            if (SDL_IntersectRectAndLine(&(allEntities.at(x)->getProHitBox().getRect()), &aX, &aY , &bX, &bY)) {
                std::cout << "Shot target at (" << aX << ", " << aY << ")" << std::endl;
                targetsInSights.push(&(allEntities.at(x)->getProHitBox()));
            }
        //}

    }
    return targetsInSights;
}
