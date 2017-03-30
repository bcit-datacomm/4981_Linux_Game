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

    checkForTargetsInVector(originX, originY, endX, endY, targetList, zombies, TYPE_ZOMBIE);
    checkForTargetsInVector(originX, originY, endX, endY, targetList, turrets, TYPE_TURRET);
    checkForTargetsInVector(originX, originY, endX, endY, targetList, walls,   TYPE_WALL);

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
        const int originY,
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
void CollisionHandler::checkForTargetsInVector(const int originX, const int originY, const int endX, const int endY,
        TargetList &targetList, std::vector<Entity*> &allEntities, int type) {

    for(auto& possibleTarget : allEntities) {

        /* These values are initialized to the end points of a line spanning from the gun muzzle 
        to the point at the end of the guns range. After SDL_IntersectRectAndLine is called
        they are changed to the end points of a line that intersects the hitbox starting with
        the entrance wound and ending with the exit wound as if the bullet were to pass straight
        through the hitbox and exit on the other side while maintaing its starting trajectory.
        This is why they are not const as the function has to be able to change them. */
        int entranceWoundX = originX;
        int entranceWoundY = originY;
        int exitWoundX = endX;
        int exitWoundY = endY;

        if (SDL_IntersectRectAndLine(&(possibleTarget->getProHitBox().getRect()),
                &entranceWoundX, &entranceWoundY , &exitWoundX, &exitWoundY)) {

            //the change in x and y from the firing origin to the spot the bullet hits the target.
            int localDeltaX = entranceWoundX - originX;
            int localDeltaY = entranceWoundY - originY;
            //the direct distance from the firing origin to the spot the bullet hits each target.
            int distanceToOrigin = std::hypot(localDeltaX, localDeltaY);

            Target tar(possibleTarget->getId(), type, entranceWoundX, entranceWoundY, distanceToOrigin);
            targetList.addTarget(tar);

            logv(3, "CollisionHandler::checkTargets() Intersect target at (%d, %d)\n", entranceWoundX, entranceWoundY);
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
