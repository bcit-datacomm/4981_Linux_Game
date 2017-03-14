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

    HitBox *detectDamageCollision(const Entity *entity);    // Check for damage collisions, return object if hits
    HitBox *detectProjectileCollision(const Entity *entity); // Check for projectile collisions, return object if hits
    bool detectMovementCollision(const Entity *entity); // // Check for collisions during movement
    Entity *detectPickUpCollision(const Entity *entity);//check for pick up collision, return object if can pick up
    std::priority_queue<HitBox*> detectLineCollision(Marine &marine, const int range);


    Quadtree quadtreeMov;
    Quadtree quadtreePro;
    Quadtree quadtreeDam;
    Quadtree quadtreePickUp;

    CollisionHandler& operator=(const CollisionHandler& handle);

private:

};


#endif
