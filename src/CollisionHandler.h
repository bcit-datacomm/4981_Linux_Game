#ifndef COLLISION_H
#define COLLISION_H
#include "HitBox.h"
#include <vector>

class CollisionHandler {
public:
	
	CollisionHandler();
	~CollisionHandler();
	
	HitBox* detectProjectileCollision(HitBox* hb);
	bool detectMovementCollisionPlayer(HitBox* hb);
	bool detectMovementCollision(HitBox* hb);
	
	void updateMoveColliders(std::vector<HitBox*> newMovementHitboxes);
	
	
private:

	std::vector<HitBox*> moveColliders;
	
};


#endif
