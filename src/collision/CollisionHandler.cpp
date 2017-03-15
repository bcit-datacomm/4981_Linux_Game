#include "Quadtree.h"
#include "CollisionHandler.h"
#include "../player/Marine.h"
#include "../log/log.h"
#include <iostream>
#include <cassert>

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
    for (unsigned int x = 0, len = returnObjects.size(); x < len; x++) {
        //assert(entity->damageHitBox->getRect().w != 0);
        //assert(entity->damageHitBox->getRect().h != 0);
        if (returnObjects.at(x) != nullptr && entity != returnObjects.at(x)
            && SDL_HasIntersection(&entity->getDamHitBox().getRect(), &returnObjects.at(x)->getDamHitBox().getRect())
                && !(entity->getDamHitBox().isPlayerFriendly() && returnObjects.at(x)->getDamHitBox().isPlayerFriendly()) ) {
            return &(returnObjects.at(x)->getDamHitBox());

        }
    }
    return nullptr;
}

// Check for projectile collisions, return object it hits
const HitBox *CollisionHandler::detectProjectileCollision(std::vector<Entity*> returnObjects, const Entity *entity) {
    for (unsigned int x = 0, len = returnObjects.size(); x < len; x++) {
        //assert(entity->projectileHitBox->getRect().w != 0);
        //assert(entity->projectileHitBox->getRect().h != 0);
        if (returnObjects.at(x) != nullptr && entity != returnObjects.at(x)
            && SDL_HasIntersection(&entity->getProHitBox().getRect(), &returnObjects.at(x)->getProHitBox().getRect())
                && !(entity->getProHitBox().isPlayerFriendly() && returnObjects.at(x)->getProHitBox().isPlayerFriendly()) ) {
            return &(returnObjects.at(x)->getProHitBox());
        }
    }
    return nullptr;
}

// Check for collisions during movement
bool CollisionHandler::detectMovementCollision(std::vector<Entity*> returnObjects, const Entity *entity) {
    for (unsigned int x = 0, len = returnObjects.size(); x < len; x++) {
        //assert(entity->movementHitBox->getRect().w != 0);
        //assert(entity->movementHitBox->getRect().h != 0);
        //assert(&returnObjects.at(x)->movementHitBox->getRect().w != 0);
        //assert(&returnObjects.at(x)->movementHitBox->getRect().h != 0);
        if(&returnObjects.at(x)->getMoveHitBox().getRect() == nullptr){
            auto temp = returnObjects.at(x)->getMoveHitBox();
        }
        auto erect = entity->getMoveHitBox().getRect();
        auto rrect = returnObjects.at(x)->getMoveHitBox().getRect();
        if (returnObjects.at(x) != nullptr && entity != returnObjects.at(x)
            && SDL_HasIntersection(&entity->getMoveHitBox().getRect(), &returnObjects.at(x)->getMoveHitBox().getRect())
                && !(entity->getMoveHitBox().isPlayerFriendly() && returnObjects.at(x)->getMoveHitBox().isPlayerFriendly()) ) {
            return true;
        }
    }
    return false;
}

//check for pickup collision
Entity *CollisionHandler::detectPickUpCollision(std::vector<Entity*> returnObjects, const Entity *entity) {
    for (unsigned int x = 0, len = returnObjects.size(); x < len; x++) {
        if (returnObjects.at(x) != nullptr && entity != returnObjects.at(x)
            && SDL_HasIntersection(&entity->getMoveHitBox().getRect(), &returnObjects.at(x)->getMoveHitBox().getRect())
                && !(entity->getMoveHitBox().isPlayerFriendly() && returnObjects.at(x)->getMoveHitBox().isPlayerFriendly()) ) {
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

    //split into check for zombies (check for walls)
    //allEntities = quadtreePro.objects;

    std::priority_queue<const HitBox*> targetsInSights;
    for (unsigned int x = 0, len = allEntities.size(); x < len; x++) {
            aX = playerX;
            aY = playerY;
            bX = aX + deltaX;
            bY = aY + deltaY;

            if (SDL_IntersectRectAndLine(&(allEntities.at(x)->getProHitBox().getRect()), &aX, &aY , &bX, &bY)) {
                logv("Shot target at (%d, %d)\n", aX, aY);
                targetsInSights.push(&(allEntities.at(x)->getProHitBox()));
            }
    }
    return targetsInSights;
}


std::vector<Entity *> CollisionHandler::getQuadTreeEntities(Quadtree &q, const Entity *entity){
    std::vector<Entity*> returnObjects;
    returnObjects = q.retrieve(entity);
    return returnObjects;
}
