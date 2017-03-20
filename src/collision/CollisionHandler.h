#ifndef COLLISION_H
#define COLLISION_H
#include "HitBox.h"
#include "Quadtree.h"
#include <vector>
#include <queue>
#include "../inventory/weapons/Target.h"


class Marine;

class CollisionHandler {
public:
    CollisionHandler();
    ~CollisionHandler() = default;


    const HitBox *detectDamageCollision(std::vector<Entity*> returnObjects, const Entity *entity);    // Check for damage collisions, return object if hits
    const HitBox *detectProjectileCollision(std::vector<Entity*> returnObjects, const Entity *entity); // Check for projectile collisions, return object if hits
    bool detectMovementCollision(const std::vector<Entity*> returnObjects, const Entity *entity); // // Check for collisions during movement
    Entity *detectPickUpCollision(std::vector<Entity*> returnObjects, const Entity *entity);//check for pick up collision, return object if can pick up
    void detectLineCollision(std::priority_queue<Target>& targetsInSights, 
        Marine &marine, const int range);
    void checkTargets(const int playerX, const int playerY, const int deltaX, const int deltaY,
        std::priority_queue<Target>& targetsInSights, std::vector<Entity*> allEntities, int type);

    std::vector<Entity *>getQuadTreeEntities(Quadtree &q,const Entity *entity); // General Collision handler, pass in quadtree check

    Quadtree quadtreeMarine; //can take dmg
    Quadtree quadtreeZombie; //can take dmg
    Quadtree quadtreeBarricade; //can take dmg
    Quadtree quadtreeTurret;
    Quadtree quadtreeWall;
    Quadtree quadtreePickUp;
    Quadtree quadtreeObj;

    CollisionHandler& operator=(const CollisionHandler& handle);

private:

};


#endif
