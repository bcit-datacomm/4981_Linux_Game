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


/**
    detectLineCollision

    DISCRIPTION:
        This checks each relevent quadTrees for targets that are in the weapons sights,
        and adds them to a priority queue that is sorted by distance from the player.

    AUTHOR: Deric Mccadden 3/8/2017
        Edited: 3/16/2017 fixed to use new quad trees (cant shoot walls yet)
        Edited: 3/17/2017 walls work now.

    PARAMS:
        TargetList &targetList,
            This is the priority queue wrapper that will hold the targets that will be determined.
        Movable &moveble,
            The movable that fired the weapon, this is needed for its x and y and angle.
        const int range
            The range of the weapon that is being fired.
*/
void CollisionHandler::detectLineCollision(TargetList &targetList, Movable& movable, const int range){

    const double degrees = movable.getAngle() - 90;
    const double radians = degrees * M_PI / 180;
    const int originX = movable.getX() + (MARINE_WIDTH / 2);
    const int originY = movable.getY() + (MARINE_HEIGHT / 2);
    const int deltaX = range * cos(radians);
    const int deltaY = range * sin(radians);
    const int endX = originX + deltaX;
    const int endY = originY + deltaY;

    targetList.setOriginX(originX);
    targetList.setOriginY(originY);
    targetList.setEndX(endX);
    targetList.setEndY(endY);

    auto& zombies = quadtreeZombie.objects;
    auto& turrets = quadtreeTurret.objects;
    auto& walls   = quadtreeWall.objects;

    checkTargets(originX, originY, endX, endY, targetList, zombies, TYPE_ZOMBIE);
    checkTargets(originX, originY, endX, endY, targetList, turrets, TYPE_TURRET);
    checkTargets(originX, originY, endX, endY, targetList, walls,   TYPE_WALL);

    logv(3, "CollisionHandler::detectLineCollision() targetsInSights.size(): %d\n", targetList.numTargets());
}

/**
    checkTargets

    DISCRIPTION:
        This checks a single vector of *hitboxes for targets that are in the weapons sights,
        and adds them to a priority queue that is sorted by distance from the player.

    AUTHOR: Deric Mccadden 3/16/2017

    PARAMS:
        const int originX,
            The starting x.
        const int originY,
            The starting y.
        const int deltaX,
            The x distance to the end point from the origin.
        const int deltaY,
            The y distance to the end point from the origin.
        TargetList &targetList,
            This is the priority queue wrapper that will hold the targets that will be determined.
        std::vector<Entity*> allEntities,
            The entities that will need to be searched for valid targets.
        int type
            the type of entity, see target Target.h for definitions.
            This is needed for identification purposes in InstantWeapon.fire()
*/
void CollisionHandler::checkTargets(const int originX, const int originY, const int endX, const int endY,
        TargetList &targetList, std::vector<Entity*> &allEntities, int type) {

    for(auto& possibleTarget : allEntities) {

        int tempOriginX = originX;
        int tempOriginY = originY;
        int tempEndX = endX;
        int tempEndY = endY;

        if (SDL_IntersectRectAndLine(&(possibleTarget->getProHitBox().getRect()),
                &tempOriginX, &tempOriginY , &tempEndX, &tempEndY)) {

            //the change in x and y from the firing origin to the spot the bullet hits the target.
            int localDeltaX = tempOriginX - originX;
            int localDeltaY = tempOriginY - originY;
            //the direct distance from the firing origin to the spot the bullet hits each target.
            int distanceToOrigin = std::hypot(localDeltaX, localDeltaY);

            Target tar(possibleTarget->getId(), type, tempOriginX, tempOriginY, distanceToOrigin);
            targetList.addTarget(tar);

            logv(3, "CollisionHandler::checkTargets() Intersect target at (%d, %d)\n", tempEndX, tempEndY);
            logv(3, "CollisionHandler::checkTargets() distanceToOrigin %d\n", distanceToOrigin);
            logv(3, "CollisionHandler::checkTargets() tar.getType(): %d\n", tar.getType());
        }
    }
}

std::vector<Entity *> CollisionHandler::getQuadTreeEntities(Quadtree& q, const Entity *entity) {
    return q.retrieve(entity);
}
