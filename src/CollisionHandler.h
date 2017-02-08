#ifndef COLLISION_H
#define COLLISION_H
#include "HitBox.h"
#include "Quadtree.h"
#include <vector>

/*
 * Work In Progress, Quadtree not implemented
 */

class CollisionHandler {
public:
	
	CollisionHandler();
	~CollisionHandler();
	
	HitBox* detectDamageCollision(HitBox* hb);	// Check for damage collisions, return object if hits
	HitBox* detectProjectileCollision(HitBox* hb); // Check for projectile collisions, return object if hits
	bool detectMovementCollision(HitBox* hb); // // Check for collisions during movement

	
	Quadtree* quadtreeMov = NULL;
	Quadtree* quadtreePro = NULL;
	Quadtree* quadtreeDam = NULL;
	
private:
	
};


#endif
