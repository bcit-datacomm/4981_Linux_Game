#include "CollisionHandler.h"

CollisionHandler::CollisionHandler() {
    quadtreeMov = new Quadtree(0, {0,0,2000,2000});
    quadtreePro = new Quadtree(0, {0,0,2000,2000});
    quadtreeDam = new Quadtree(0, {0,0,2000,2000});
    quadtreePickUp = new Quadtree(0, {0,0,2000,2000});
}

CollisionHandler::~CollisionHandler() {
    delete quadtreeMov;
    delete quadtreePro;
    delete quadtreeDam;
    delete quadtreePickUp;
}

// Check for projectile collisions, return object it hits
HitBox* CollisionHandler::detectDamageCollision(HitBox* hb) {
    std::vector<HitBox*> returnObjects;
    returnObjects = quadtreeDam->retrieve(returnObjects, hb);
      for (unsigned int x = 0; x < returnObjects.size(); x++) {
        if (returnObjects.at(x) != nullptr && hb->attached != returnObjects.at(x)->attached) {
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
    returnObjects = quadtreePro->retrieve(returnObjects, hb);
      for (unsigned int x = 0; x < returnObjects.size(); x++) {
        if (returnObjects.at(x) != nullptr && hb->attached != returnObjects.at(x)->attached) {
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
    returnObjects = quadtreeMov->retrieve(returnObjects, hb);
      for (unsigned int x = 0; x < returnObjects.size(); x++) {
        if (returnObjects.at(x) != nullptr && hb->attached != returnObjects.at(x)->attached) {
            if (SDL_HasIntersection(&hb->getRect(), &returnObjects.at(x)->getRect()) &&
                !(hb->isPlayerFriendly() && returnObjects.at(x)->isPlayerFriendly()) ) {
                return true;
            }
        }
      }
    return false;
}

//check for pickup collision
HitBox* CollisionHandler::detectPickUpCollision(HitBox* hb) {
    std::vector<HitBox*> returnObjects;
    returnObjects = quadtreePickUp->retrieve(returnObjects, hb);
      for (unsigned int x = 0; x < returnObjects.size(); x++) {
        if (returnObjects.at(x) != nullptr && hb->attached != returnObjects.at(x)->attached) {
            if (SDL_HasIntersection(&hb->getRect(), &returnObjects.at(x)->getRect()) &&
                !(hb->isPlayerFriendly() && returnObjects.at(x)->isPlayerFriendly()) ) {
                return returnObjects.at(x);
            }
        }
      }
    return nullptr;
}

