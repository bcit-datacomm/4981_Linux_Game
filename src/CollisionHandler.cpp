#include "Quadtree.h"
#include "CollisionHandler.h"

CollisionHandler::CollisionHandler() : quadtreeMov(0, {0,0,2000,2000}), quadtreePro(0, {0,0,2000,2000}), 
        quadtreeDam(0, {0,0,2000,2000}),quadtreePickUp(0, {0,0,2000,2000}) {

}

CollisionHandler& CollisionHandler::operator=(const CollisionHandler& handle) {
    quadtreeMov = handle.quadtreeMov;
    quadtreePro = handle.quadtreePro;
    quadtreeDam = handle.quadtreeDam;
    quadtreePickUp = handle.quadtreePickUp;
    return *this;
}

// Check for projectile collisions, return object it hits
HitBox *CollisionHandler::detectDamageCollision(const HitBox *hb) {
    std::vector<HitBox *> returnObjects;
    returnObjects = quadtreeDam.retrieve(hb);
    for (unsigned int x = 0; x < returnObjects.size(); x++) {
        if (returnObjects.at(x) != nullptr && hb->attached != returnObjects.at(x)->attached 
            && SDL_HasIntersection(&hb->getRect(), &returnObjects.at(x)->getRect()) 
                && !(hb->isPlayerFriendly() && returnObjects.at(x)->isPlayerFriendly()) ) {
            return returnObjects.at(x);
        }
    }
    return nullptr;
}

// Check for projectile collisions, return object it hits
HitBox *CollisionHandler::detectProjectileCollision(const HitBox *hb) {
    std::vector<HitBox*> returnObjects;
    returnObjects = quadtreePro.retrieve(hb);
    for (unsigned int x = 0; x < returnObjects.size(); x++) {
        if (returnObjects.at(x) != nullptr && hb->attached != returnObjects.at(x)->attached 
            && SDL_HasIntersection(&hb->getRect(), &returnObjects.at(x)->getRect()) 
                && !(hb->isPlayerFriendly() && returnObjects.at(x)->isPlayerFriendly()) ) {
            return returnObjects.at(x);
        }
    }
    return nullptr;
}

// Check for collisions during movement
bool CollisionHandler::detectMovementCollision(const HitBox *hb) {
    std::vector<HitBox *> returnObjects;
    returnObjects = quadtreeMov.retrieve(hb);
    for (unsigned int x = 0; x < returnObjects.size(); x++) {
        if (returnObjects.at(x) != nullptr && hb->attached != returnObjects.at(x)->attached 
            && SDL_HasIntersection(&hb->getRect(), &returnObjects.at(x)->getRect()) 
                && !(hb->isPlayerFriendly() && returnObjects.at(x)->isPlayerFriendly()) ) {
            return true;
        }
    }
    return false;
}

//check for pickup collision
HitBox *CollisionHandler::detectPickUpCollision(const HitBox *hb) {
    std::vector<HitBox*> returnObjects;
    returnObjects = quadtreeMov.retrieve(hb);
    for (unsigned int x = 0; x < returnObjects.size(); x++) {
        if (hb->attached != returnObjects.at(x)->attached 
            && SDL_HasIntersection(&hb->getRect(), &returnObjects.at(x)->getRect()) 
                && !(hb->isPlayerFriendly() && returnObjects.at(x)->isPlayerFriendly()) ) {
            return returnObjects.at(x);
        }
    }
    return nullptr;
}
