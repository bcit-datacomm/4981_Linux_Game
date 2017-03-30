#include "Quadtree.h"
#include "CollisionHandler.h"
#include "../player/Marine.h"
#include "../log/log.h"
#include <iostream>
#include <cmath>
#include <cassert>
#include "../inventory/weapons/Target.h"

/**
 * Date: Mar. 1, 2017
 * Modified: Mar. 15, 2017 - Mark Tattrie
 * Author: Jacob McPhail.
 * Function Interface: CollisionHandler::CollisionHandler()
 * Description:
 * Constructor for Collision Handler
 */
CollisionHandler::CollisionHandler() : quadtreeMarine(0, {0,0,2000,2000}), quadtreeZombie(0, {0,0,2000,2000}),
        quadtreeBarricade(0, {0,0,2000,2000}),quadtreeTurret(0, {0,0,2000,2000}),
        quadtreeWall(0, {0,0,2000,2000}), quadtreePickUp(0, {0,0,2000,2000}), quadtreeObj(0, {0,0,2000,2000}) {

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
    return *this;
}

/**
 * Date: Mar. 1, 2017
 * Modified: Mar. 15 2017 - Mark Tattrie
 * Author: Jacob McPhail.
 * Function Interface: const HitBox *CollisionHandler::detectDamageCollision(std::vector<Entity*>
 *      returnObjects, const Entity *entity) {
 * Description:
 * Check for projectile collisions, return hitbox it hits
 */
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

/**
 * Date: Mar. 1, 2017
 * Modified: Mar. 15 2017 - Mark Tattrie
 * Author: Jacob McPhail.
 * Function Interface: const HitBox *CollisionHandler::detectProjectileCollision(std::vector<Entity*>
 *      returnObjects, const Entity *entity) {
 * Description:
 * Check for projectile collisions, return object it hits
 */
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

/**
 * Date: Mar. 1, 2017
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
        std::priority_queue<Target>& targetsInSights,
            This is the queue that will hold the targets that will be determined.
        Marine &marine,
            The marine that fired the weapon, this is needed for its x and y and angle.
        const int range
            The range of the weapon that is being fired.
*/
void CollisionHandler::detectLineCollision(
        std::priority_queue<Target>& targetsInSights, Marine& marine, const int range) {

    const double degrees = marine.getAngle() - 90;
    const double radians = degrees * M_PI / 180;
    const int originX = marine.getX() + (MARINE_WIDTH / 2);
    const int originY = marine.getY() + (MARINE_HEIGHT / 2);
    const int deltaX  = range * cos(radians);
    const int deltaY  = range * sin(radians);

    auto& zombies = quadtreeZombie.objects;
    auto& turrets = quadtreeTurret.objects;
    auto& walls   = quadtreeWall.objects;

    checkTargets(originX, originY, deltaX, deltaY, targetsInSights, zombies, TYPE_ZOMBIE);
    checkTargets(originX, originY, deltaX, deltaY, targetsInSights, turrets, TYPE_TURRET);
    checkTargets(originX, originY, deltaX, deltaY, targetsInSights, walls, TYPE_WALL);

    logv("targetsInSights.size(): %d\n", targetsInSights.size());
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
        std::priority_queue<Target>& targetsInSights,
            This is the queue that will hold the targets that will be determined.
        std::vector<Entity*> allEntities,
            The entities that will need to be searched for valid targets.
        int type
            the type of entity, see target Target.h for definitions.
            This is needed for identification purposes in InstantWeapon.fire()
*/
void CollisionHandler::checkTargets(const int originX, const int originY, const int deltaX, const int deltaY,
        std::priority_queue<Target>& targetsInSights, std::vector<Entity*>& allEntities, int type) {

    int tempOriginX = originX;
    int tempOriginY = originY;
    int tempEndX = originX + deltaX;
    int tempEndY = originY + deltaY;
    int distanceToOrigin;

    for(auto& possibleTarget : allEntities) {

        if (SDL_IntersectRectAndLine(&(possibleTarget->getProHitBox().getRect()),
                &tempOriginX, &tempOriginY , &tempEndX, &tempEndY)) {

            logv("Intersect target at (%d, %d)\n", tempEndX, tempEndY);

            distanceToOrigin = std::hypot(deltaX, deltaY);

            Target tar(possibleTarget->getId(), type, tempEndX, tempEndY, distanceToOrigin);

            logv("tar.getType(): %d\n", tar.getType());

            targetsInSights.push(tar);
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
