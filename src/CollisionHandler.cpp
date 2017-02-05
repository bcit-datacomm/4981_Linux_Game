#include "CollisionHandler.h"

CollisionHandler::CollisionHandler() {
	//this->gameManager = gm;	
}

CollisionHandler::~CollisionHandler() {
	
}
	
HitBox* CollisionHandler::detectProjectileCollision(HitBox* hb) {
	return NULL;
}

bool CollisionHandler::detectMovementCollisionPlayer(HitBox* hb) {
	return false;
}
	
bool CollisionHandler::detectMovementCollision(HitBox* hb) {
	/*for (const auto& m : this->gameManager->marineManager) {
		if (hb->attached != m.second->movementHitBox.attached) {
			if (SDL_HasIntersection(&hb->getRect(), &m.second->movementHitBox.getRect())) {
				return true;
			}
		}
	}*/
	for (HitBox* hb2 : this->moveColliders) {
		if (hb->attached != hb2->attached) {
			if (SDL_HasIntersection(&hb->getRect(), &hb2->getRect())) {
				return true;
			}
		}
	}
	return false;
}

void CollisionHandler::updateMoveColliders(std::vector<HitBox*> newMovementHitboxes) {
	this->moveColliders.clear();
	this->moveColliders = newMovementHitboxes;
}