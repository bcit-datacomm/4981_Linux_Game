/*------------------------------------------------------------------------------
* Header: CollisionHandler.cpp
*
* Functions:
*
*
* Date:
*
* Revisions:
* Edited By : Justen DePourcq- Style guide
*
* Designer:
*
* Author:
*
* Notes:
*
------------------------------------------------------------------------------*/
#include "Quadtree.h"
#include "CollisionHandler.h"
#include "../player/Marine.h"
#include "../log/log.h"
#include <iostream>
#include <cmath>
#include <cassert>
#include "../inventory/weapons/Target.h"

/**
 * Date: Feb. 4, 2017
 * Modified: Mar. 15, 2017 - Mark Tattrie
 * Author: Jacob McPhail.
 * Function Interface: CollisionHandler::CollisionHandler()
 * Description:
 * Constructor for Collision Handler
 */
CollisionHandler::CollisionHandler() : quadtreeMarine(0, {0,0,2000,2000}), quadtreeZombie(0, {0,0,2000,2000}),
        quadtreeBarricade(0, {0,0,2000,2000}),quadtreeTurret(0, {0,0,2000,2000}),
        quadtreeWall(0, {0,0,2000,2000}), quadtreePickUp(0, {0,0,2000,2000}), quadtreeObj(0, {0,0,2000,2000}),
        quadtreeStore(0,{0,0,2000,2000}) {

}

/**
 * Date: Mar. 1, 2017
 * Modified: Mar. 15 2017 - Mark Tattrie
 * Author: Jacob McPhail.
 * Function Interface: CollisionHandler& CollisionHandler::operator=(const CollisionHandler& handle)
 * Description:
 * Comparison operator for = to set each Quadtree
 */
CollisionHandler& CollisionHandler::operator=(const CollisionHandler& handle) {
    quadtreeMarine = handle.quadtreeMarine;
    quadtreeZombie = handle.quadtreeZombie;
    quadtreeBarricade = handle.quadtreeBarricade;
    quadtreeTurret = handle.quadtreeTurret;
    quadtreeWall = handle.quadtreeWall;
    quadtreePickUp = handle.quadtreePickUp;
    quadtreeObj = handle.quadtreeObj;
    quadtreeStore = handle.quadtreeStore;
    return *this;
}

/**
 * Date: Feb. 4, 2017
 * Modified: Mar. 15 2017 - Mark Tattrie
 * Author: Jacob McPhail.
 * Function Interface: const HitBox *CollisionHandler::detectDamageCollision(std::vector<Entity*>
 *      returnObjects, const Entity *entity) {
 * Description:
 * Check for projectile collisions, return hitbox it hits
 */
const HitBox *CollisionHandler::detectDamageCollision(std::vector<Entity*> returnObjects,
        const Entity *entity) {
    for (const auto& obj: returnObjects) {
        if (obj != nullptr && entity != obj
            && SDL_HasIntersection(&entity->getDamHitBox().getRect(), &obj->getDamHitBox().getRect())
                && !(entity->getDamHitBox().isPlayerFriendly() && obj->getDamHitBox().isPlayerFriendly())) {
            return &(obj->getDamHitBox());
        }
    }
    return nullptr;
}

/**
 * Date: Feb. 4, 2017
 * Modified: Mar. 15 2017 - Mark Tattrie
 * Author: Jacob McPhail.
 * Function Interface: const HitBox *CollisionHandler::detectProjectileCollision(std::vector<Entity*>
 *      returnObjects, const Entity *entity) {
 * Description:
 * Check for projectile collisions, return object it hits
 */
const HitBox *CollisionHandler::detectProjectileCollision(std::vector<Entity*> returnObjects,
        const Entity *entity) {
    for (const auto& obj: returnObjects) {
        if (obj != nullptr && entity != obj
            && SDL_HasIntersection(&entity->getProHitBox().getRect(), &obj->getProHitBox().getRect())
                && !(entity->getProHitBox().isPlayerFriendly() && obj->getProHitBox().isPlayerFriendly())) {
            return &(obj->getProHitBox());
        }
    }
    return nullptr;
}

/**
 * Date: Feb. 4, 2017
 * Modified: Mar. 15 2017 - Mark Tattrie
 * Author: Jacob McPhail.
 * Function Interface: bool CollisionHandler::detectMovementCollision(std::vector<Entity*> returnObjects,
 *       const Entity *entity)
 * Description:
 * Check for collisions during movement
 */
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

/**
 * Date: Mar. 1, 2017
 * Modified: Mar. 15 2017 - Mark Tattrie
 * Author: Maitiu Morton.
 * Function Interface: Entity *CollisionHandler::detectPickUpCollision(std::vector<Entity*> returnObjects,
 *       const Entity *entity)
 * Description:
 * Check for pickup collision
 */
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

        const int gunX, const int gunY,
            coordinates of the weapons muzzle.

        const double angle
            angle the weapon is facing.

        const int range
            The range of the weapon that is being fired.
*/
void CollisionHandler::detectLineCollision(TargetList& targetList, const int gunX, const int gunY,
        const double angle, const int range) {

    const double degrees = angle - 90;
    const double radians = degrees * M_PI / 180;
    const int deltaX = range * cos(radians);
    const int deltaY = range * sin(radians);
    const int endX = gunX + deltaX;
    const int endY = gunY + deltaY;

    targetList.setOriginX(gunX);
    targetList.setOriginY(gunY);
    targetList.setEndX(endX);
    targetList.setEndY(endY);

    auto& zombies = quadtreeZombie.objects;
    auto& turrets = quadtreeTurret.objects;
    auto& walls   = quadtreeWall.objects;

    checkForTargetsInVector(gunX, gunY, endX, endY, targetList, zombies, TYPE_ZOMBIE);
    checkForTargetsInVector(gunX, gunY, endX, endY, targetList, turrets, TYPE_TURRET);
    checkForTargetsInVector(gunX, gunY, endX, endY, targetList, walls,   TYPE_WALL);

    logv(3, "CollisionHandler::detectLineCollision() targetsInSights.size(): %d\n", targetList.numTargets());
}

/**
 * Date: Mar. 28, 2017
 * Author: Mark Tattrie
 * Function Interface: std::vector<Entity *> CollisionHandler::detectMeleeCollision(
 *      std::vector<Entity*> returnObjects, const Entity *entity, const HitBox hb)
 * Description:
 * returns a vector of entities that have a damage hitbox collision between the vector of entities
 * and the entity you pass in
 */
std::vector<Entity *> CollisionHandler::detectMeleeCollision(std::vector<Entity*> returnObjects,
        const Entity *entity, const HitBox hb) {
    std::vector<Entity *> allEntities;
    for (const auto& obj: returnObjects) {
        if (obj && entity != obj && SDL_HasIntersection(&hb.getRect(), &obj->getDamHitBox().getRect())) {
            allEntities.push_back(obj);
        }
    }
    return allEntities;
}

/**
    checkTargets

    DISCRIPTION:
        This checks a single vector of *hitboxes for targets that are in the weapons sights,
        and adds them to a priority queue that is sorted by distance from the player.

    AUTHOR: Deric Mccadden 3/16/2017

    PARAMS:
        const int gunX,
        const int gunY,
            The x and y where the bullet is fired from.

        const int endX,
        const int endY,
            The furthest point the bullet can travel.

        TargetList &targetList,
            This is the priority queue wrapper that will hold the targets that will be determined.

        std::vector<Entity*> allEntities,
            The entities that will need to be searched for valid targets.

        int type
            the type of entity, see target Target.h for definitions.
            This is needed for identification purposes in InstantWeapon.fire()
*/
void CollisionHandler::checkForTargetsInVector(const int gunX, const int gunY, const int endX, const int endY,
        TargetList& targetList, std::vector<Entity *>& allEntities, int type) {

    for(const auto& possibleTarget : allEntities) {

        /* These values are initialized to the end points of a line spanning from the gun muzzle
        to the point at the end of the guns range. After SDL_IntersectRectAndLine is called
        they are changed to the end points of a line that intersects the hitbox starting with
        the entrance wound and ending with the exit wound as if the bullet were to pass straight
        through the hitbox and exit on the other side while maintaing its starting trajectory.
        This is why they are not const as the function has to be able to change them. */
        int entranceWoundX = gunX;
        int entranceWoundY = gunY;
        int exitWoundX = endX;
        int exitWoundY = endY;

        if (SDL_IntersectRectAndLine(&(possibleTarget->getProHitBox().getRect()),
                &entranceWoundX, &entranceWoundY , &exitWoundX, &exitWoundY)) {

            //the change in x and y from the firing origin to the spot the bullet hits the target.
            int localDeltaX = entranceWoundX - gunX;
            int localDeltaY = entranceWoundY - gunY;
            //the direct distance from the firing origin to the spot the bullet hits each target.
            int distanceToOrigin = std::hypot(localDeltaX, localDeltaY);

            Target tar(possibleTarget->getId(), type, entranceWoundX, entranceWoundY, distanceToOrigin);
            targetList.addTarget(tar);

            logv(3, "CollisionHandler::checkTargets() Intersect target at (%d, %d)\n",
                entranceWoundX, entranceWoundY);
            logv(3, "CollisionHandler::checkTargets() distanceToOrigin %d\n", distanceToOrigin);
            logv(3, "CollisionHandler::checkTargets() tar.getType(): %d\n", tar.getType());
        }
    }
}


/**
 * Date: Mar. 15, 2017
 * Author: Mark Tattrie
 * Function Interface: std::vector<Entity *> CollisionHandler::getQuadTreeEntities(Quadtree& q,
 *      const Entity *entity)
 * Description:
 * Wrapper to grab a vector of entities from the specified quadtree
 */
std::vector<Entity *> CollisionHandler::getQuadTreeEntities(Quadtree& q, const Entity *entity) {
    return q.retrieve(entity);
}
