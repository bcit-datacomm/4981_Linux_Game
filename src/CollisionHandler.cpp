#include "Quadtree.h"
#include "CollisionHandler.h"
#include "Marine.h"
#include <iostream>

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
    for (unsigned int x = 0, length = returnObjects.size(); x < length; x++) {
        if (returnObjects[x] != nullptr && hb->attached != returnObjects[x]->attached 
            && SDL_HasIntersection(&hb->getRect(), &returnObjects[x]->getRect()) 
                && !(hb->isPlayerFriendly() && returnObjects[x]->isPlayerFriendly()) ) {
            return returnObjects[x];
        }
    }
    return nullptr;
}

// Check for projectile collisions, return object it hits
HitBox *CollisionHandler::detectProjectileCollision(const HitBox *hb) {
    std::vector<HitBox*> returnObjects;
    returnObjects = quadtreePro.retrieve(hb);
    for (unsigned int x = 0, length = returnObjects.size(); x < length; x++) {
        if (returnObjects[x] != nullptr && hb->attached != returnObjects[x]->attached 
            && SDL_HasIntersection(&hb->getRect(), &returnObjects[x]->getRect()) 
                && !(hb->isPlayerFriendly() && returnObjects[x]->isPlayerFriendly()) ) {
            return returnObjects[x];
        }
    }
    return nullptr;
}

// Check for collisions during movement
bool CollisionHandler::detectMovementCollision(const HitBox *hb) {
    std::vector<HitBox *> returnObjects;
    returnObjects = quadtreeMov.retrieve(hb);
    for (unsigned int x = 0, length = returnObjects.size(); x < length; x++) {
        if (returnObjects[x] != nullptr && hb->attached != returnObjects[x]->attached 
            && SDL_HasIntersection(&hb->getRect(), &returnObjects[x]->getRect()) 
                && !(hb->isPlayerFriendly() && returnObjects[x]->isPlayerFriendly()) ) {
            return true;
        }
    }
    return false;
}

//check for pickup collision
HitBox *CollisionHandler::detectPickUpCollision(const HitBox *hb) {
    std::vector<HitBox*> returnObjects;
    returnObjects = quadtreeMov.retrieve(hb);
    for (unsigned int x = 0, length = returnObjects.size(); x < length; x++) {
        if (returnObjects[x] != nullptr && hb->attached != returnObjects[x]->attached 
            && SDL_HasIntersection(&hb->getRect(), &returnObjects[x]->getRect()) 
                && !(hb->isPlayerFriendly() && returnObjects[x]->isPlayerFriendly()) ) {
            return returnObjects[x];
        }
    }
    return nullptr;
}


// Created by DericM 3/8/2017
std::priority_queue<HitBox*> CollisionHandler::detectLineCollision(
        Marine &marine, const int range) {
    
    const double degrees = marine.getAngle() - 90;
    const double radians = degrees * M_PI / 180;
    const int playerX = marine.getX() + (MARINE_WIDTH / 2);
    const int playerY = marine.getY() + (MARINE_HEIGHT / 2);
    const int deltaX  = range * cos(radians);
    const int deltaY  = range * sin(radians);
    int aX, aY, bX, bY;
        
    std::vector<HitBox *> allHitBoxes = quadtreePro.objects;

    std::priority_queue<HitBox*> targetsInSights;
    for (unsigned int x = 0; x < allHitBoxes.size(); x++) {
        if (allHitBoxes.at(x)->attached != marine.projectileHitBox->attached) {
            aX = playerX;
            aY = playerY;
            bX = aX + deltaX;
            bY = aY + deltaY;

            if (SDL_IntersectRectAndLine(&allHitBoxes.at(x)->getRect(), &aX, &aY , &bX, &bY) &&
                    !(allHitBoxes.at(x)->isPlayerFriendly()) ) {
                std::cout << "Shot target at (" << aX << ", " << aY << ")" << std::endl;
                targetsInSights.push(allHitBoxes.at(x));
            }
        }
        
    }
    
    return targetsInSights;
}
