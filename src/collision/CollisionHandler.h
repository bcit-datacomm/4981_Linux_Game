/*------------------------------------------------------------------------------
* Header: CollisionHandler.h
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
#ifndef COLLISION_H
#define COLLISION_H

#include <vector>
#include <queue>

#include "HitBox.h"
#include "Quadtree.h"
#include "../inventory/weapons/Target.h"

class Movable;

class CollisionHandler {
public:
    CollisionHandler();
    ~CollisionHandler() = default;

    // Check for damage collisions, return object if hits
    const HitBox *detectDamageCollision(std::vector<Entity*> returnObjects, const Entity *entity);

    // Check for projectile collisions, return object if hits
    const HitBox *detectProjectileCollision(std::vector<Entity*> returnObjects, const Entity *entity);

    //Check for collisions during movement
    bool detectMovementCollision(const std::vector<Entity*> returnObjects, const Entity *entity);

    //check for pick up collision, return object if can pick up
    Entity *detectPickUpCollision(std::vector<Entity*> returnObjects, const Entity *entity);

    void detectLineCollision(TargetList& targetList, const int gunX, const int gunY, const double angle, const int range);

    std::vector<Entity *> detectMeleeCollision(const std::vector<Entity*>& returnObjects, const Entity *entity, const HitBox hb);

    std::vector<Entity *>getQuadTreeEntities(const Quadtree& q, const Entity *entity) const; // General Collision handler, pass in quadtree check

    CollisionHandler& operator=(const CollisionHandler& handle);

private:
    void checkForTargetsInVector(const int gunX, const int gunY, const int endX, const int endY,
        TargetList& targetList, const std::vector<Entity*>& allEntities, const int type) const;


    Quadtree totalTree;

    Quadtree quadtreeMarine; //can take dmg
    Quadtree quadtreeZombie; //can take dmg
    Quadtree quadtreeBarricade; //can take dmg
    Quadtree quadtreeTurret;
    Quadtree quadtreeWall;
    Quadtree quadtreePickUp;
    Quadtree quadtreeObj;
    Quadtree quadtreeStore;
};


#endif
