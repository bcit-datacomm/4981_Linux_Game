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
    for (const auto& obj: returnObjects) {
        if (obj != nullptr && entity != obj
            && SDL_HasIntersection(&entity->getDamHitBox().getRect(), &obj->getDamHitBox().getRect())
                && !(entity->getDamHitBox().isPlayerFriendly() && obj->getDamHitBox().isPlayerFriendly())) {
            return &(obj->getDamHitBox());
        }
    }
    return nullptr;
}

// Check for projectile collisions, return object it hits
const HitBox *CollisionHandler::detectProjectileCollision(std::vector<Entity*> returnObjects, const Entity *entity) {
    for (const auto& obj: returnObjects) {
        if (obj != nullptr && entity != obj
            && SDL_HasIntersection(&entity->getProHitBox().getRect(), &obj->getProHitBox().getRect())
                && !(entity->getProHitBox().isPlayerFriendly() && obj->getProHitBox().isPlayerFriendly())) {
            return &(obj->getProHitBox());
        }
    }
    return nullptr;
}

// Check for collisions during movement
bool CollisionHandler::detectMovementCollision(std::vector<Entity*> returnObjects, const Entity *entity) {
    for (const auto& obj: returnObjects) {
        if (obj != nullptr && entity != obj
            && SDL_HasIntersection(&entity->getMoveHitBox().getRect(), &obj->getMoveHitBox().getRect())
                && !(entity->getMoveHitBox().isPlayerFriendly() && obj->getMoveHitBox().isPlayerFriendly())) {
            return true;
        }
    }
    return false;
}

//check for pickup collision
Entity *CollisionHandler::detectPickUpCollision(std::vector<Entity*> returnObjects, const Entity *entity) {
    for (const auto& obj: returnObjects) {
        if (obj != nullptr && entity != obj
            && SDL_HasIntersection(&entity->getMoveHitBox().getRect(), &obj->getPickUpHitBox().getRect())
                && !(entity->getMoveHitBox().isPlayerFriendly() && obj->getPickUpHitBox().isPlayerFriendly())) {
            return obj;
        }
    }
    logv("nothing to pick up\n");
    return nullptr;

}

// Created by DericM 3/8/2017
// TODO fix collision detection using new quadtrees
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

std::vector<Entity *> CollisionHandler::getQuadTreeEntities(Quadtree &q, const Entity *entity) {
    return q.retrieve(entity);
}
