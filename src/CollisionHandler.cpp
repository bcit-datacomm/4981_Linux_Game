#include "CollisionHandler.h"

CollisionHandler::CollisionHandler() {
	this->quadtreeMov = new Quadtree(0, {0,0,2000,2000});
	this->quadtreePro = new Quadtree(0, {0,0,2000,2000});
	this->quadtreeDam = new Quadtree(0, {0,0,2000,2000});
}

CollisionHandler::~CollisionHandler() {
	
}

// Check for projectile collisions, return object it hits
HitBox* CollisionHandler::detectDamageCollision(HitBox* hb) {
	std::vector<HitBox*> returnObjects;
	returnObjects = this->quadtreeDam->retrieve(returnObjects, hb);
  	for (unsigned int x = 0; x < returnObjects.size(); x++) {
    	if (hb->attached != returnObjects.at(x)->attached) {
			if (SDL_HasIntersection(&hb->getRect(), &returnObjects.at(x)->getRect()) &&
				!(hb->isPlayerFriendly() && returnObjects.at(x)->isPlayerFriendly()) ) {
				return returnObjects.at(x);
			}
		}
  	}
	return nullptr;
}
	
// Check for projectile collisions, return object it hits
HitBox* CollisionHandler::detectProjectileCollision(HitBox* hb) {
	std::vector<HitBox*> returnObjects;
	returnObjects = this->quadtreePro->retrieve(returnObjects, hb);
  	for (unsigned int x = 0; x < returnObjects.size(); x++) {
    	if (hb->attached != returnObjects.at(x)->attached) {
			if (SDL_HasIntersection(&hb->getRect(), &returnObjects.at(x)->getRect()) &&
				!(hb->isPlayerFriendly() && returnObjects.at(x)->isPlayerFriendly()) ) {
				return returnObjects.at(x);
			}
		}
  	}
	return nullptr;
}
	
// Check for collisions during movement
bool CollisionHandler::detectMovementCollision(HitBox* hb) {
	std::vector<HitBox*> returnObjects;
	returnObjects = this->quadtreeMov->retrieve(returnObjects, hb);
  	for (unsigned int x = 0; x < returnObjects.size(); x++) {
    	if (hb->attached != returnObjects.at(x)->attached) {
			if (SDL_HasIntersection(&hb->getRect(), &returnObjects.at(x)->getRect()) &&
				!(hb->isPlayerFriendly() && returnObjects.at(x)->isPlayerFriendly()) ) {
				return true;
			}
		}
  	}
	return false;
}

