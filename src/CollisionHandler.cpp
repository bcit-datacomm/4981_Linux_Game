#include "CollisionHandler.h"

CollisionHandler::CollisionHandler() {

}

CollisionHandler::~CollisionHandler() {
	
}
	
// Check for projectile collisions, return object it hits
HitBox* CollisionHandler::detectProjectileCollision(HitBox* hb) {
	for (HitBox* hb2 : this->projectileColliders) {
		if (hb->attached != hb2->attached) {
			if (SDL_HasIntersection(&hb->getRect(), &hb2->getRect()) && !(hb->isPlayerFriendly() && hb2->isPlayerFriendly()) ) {
				return hb2;
			}
		}
	}
	return NULL;
}
	
// Check for collisions during movement
bool CollisionHandler::detectMovementCollision(HitBox* hb) {
	for (HitBox* hb2 : this->moveColliders) {
		if (hb->attached != hb2->attached) {
			if (SDL_HasIntersection(&hb->getRect(), &hb2->getRect()) && !(hb->isPlayerFriendly() && hb2->isPlayerFriendly()) ) {
				return true;
			}
		}
	}
	return false;
}

void CollisionHandler::updateColliders(std::vector<HitBox*> newMovementHitboxes, std::vector<HitBox*> newProjectileColliders) {
	this->moveColliders.clear();
	this->moveColliders = newMovementHitboxes;
	this->projectileColliders.clear();
	this->projectileColliders = newProjectileColliders;
}