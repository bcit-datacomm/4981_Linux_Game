#ifndef COLLISION_H
#define COLLISION_H
#include "HitBox.h"
#include <vector>

/*
 * Work In Progress, Quadtree not implemented
 */

class CollisionHandler {
public:
	
	CollisionHandler();
	~CollisionHandler();
	
	HitBox* detectProjectileCollision(HitBox* hb); // Check for projectile collisions, return object it hits
	bool detectMovementCollision(HitBox* hb); // // Check for collisions during movement
	
	void updateColliders(std::vector<HitBox*> newMoveColliders, 
						std::vector<HitBox*> newProjectileColliders);
	
	
private:

	std::vector<HitBox*> moveColliders;
	std::vector<HitBox*> projectileColliders;

};


#endif
