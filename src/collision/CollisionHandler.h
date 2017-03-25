#ifndef COLLISION_H
#define COLLISION_H
#include "HitBox.h"
#include "Quadtree.h"
#include <vector>
#include <queue>

class Marine;

class CollisionHandler {
public:
    CollisionHandler();
    ~CollisionHandler() = default;

    const HitBox *detectDamageCollision(std::vector<Entity*> returnObjects, const Entity *entity);    // Check for damage collisions, return object if hits
    const HitBox *detectProjectileCollision(std::vector<Entity*> returnObjects, const Entity *entity); // Check for projectile collisions, return object if hits
    bool detectMovementCollision(const std::vector<Entity*> returnObjects, const Entity *entity); // // Check for collisions during movement
    Entity *detectPickUpCollision(std::vector<Entity*> returnObjects, const Entity *entity);//check for pick up collision, return object if can pick up
    std::priority_queue<const HitBox*> detectLineCollision(Marine &marine, const int range);

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
