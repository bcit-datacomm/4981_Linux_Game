// Created 05/02/2017 Mark C.
#include "Turret.h"
#include "../log/log.h"
#include <cassert>

Turret::Turret(int32_t id, const SDL_Rect dest, const SDL_Rect &movementSize, const SDL_Rect &projectileSize,
        const SDL_Rect &damageSize, const SDL_Rect &pickupSize, bool activated, int health, int ammo,
        bool placed): Entity(id, dest, movementSize, projectileSize, damageSize,
        pickupSize), Movable(id, dest, movementSize, projectileSize, damageSize,
        pickupSize, MARINE_VELOCITY), activated(activated), ammo(ammo), boolPlaced(placed) {
    //movementHitBox.setFriendly(true); Uncomment to allow movement through other players
    //projectileHitBox.setFriendly(true); Uncomment for no friendly fire
    //damageHitBox.setFriendly(true); Uncomment for no friendly fire
    logv("Turret created\n");
}

Turret::~Turret() {
    logv("Destroy Turret\n");
}

// checks if turret placement is within bounds, currently does nothing
bool Turret::placementCheckTurret(){
    return true;
}

// checks if the turret placement overlaps with any currently existing objects
bool Turret::collisionCheckTurret(const float playerX, const float playerY, const float moveX,
        const float moveY, CollisionHandler &ch) {
    SDL_Rect checkBox;

    checkBox.h = TURRET_HEIGHT;
    checkBox.w = TURRET_WIDTH;
    checkBox.x = moveX;
    checkBox.y = moveY;
    HitBox hitBox(checkBox);
    const float distanceX = (playerX - moveX) * (playerX - moveX);
    const float distanceY = (playerY - moveY) * (playerY - moveY);
    const float distance = sqrt(abs(distanceX + distanceY));

    /**
    return (distance <= 200 && (!ch.detectMovementCollision(ch.getQuadTreeEntities(ch.quadtreeMarine,this), this)
            //&& !ch.detectMovementCollision(ch.getQuadTreeEntities(ch.quadtreeZombie,this), this)
            && !ch.detectMovementCollision(ch.getQuadTreeEntities(ch.quadtreeBarricade,this), this)
            && !ch.detectMovementCollision(ch.getQuadTreeEntities(ch.quadtreeWall,this), this)
            && !ch.detectMovementCollision(ch.getQuadTreeEntities(ch.quadtreeTurret,this), this)
            && !ch.detectMovementCollision(ch.getQuadTreeEntities(ch.quadtreeObj,this), this)
            && !ch.detectMovementCollision(ch.getQuadTreeEntities(ch.quadtreePickUp,this), this)));
    */

    //assert(this->movementHitBox->getRect().h != 0);
    //assert(this->movementHitBox->getRect().w != 0);


    bool canPlace = false;
    if( distance <= 200){
        logv("0) distance < 200\n");
        canPlace = true;
    }
    if(ch.detectMovementCollision(ch.getQuadTreeEntities(ch.quadtreeZombie, this), this)){
        logv("1) zombie movement collision\n");
        canPlace = false;
    }
    if(ch.detectMovementCollision(ch.getQuadTreeEntities(ch.quadtreeMarine, this), this)){
        logv("2) marine movement collision\n");
        canPlace = false;
    }
    if(ch.detectMovementCollision(ch.getQuadTreeEntities(ch.quadtreeBarricade, this), this)){
        logv("3) barricade movement collision\n");
        canPlace = false;
    }
    if(ch.detectMovementCollision(ch.getQuadTreeEntities(ch.quadtreeWall, this), this)){
        logv("4) wall movement collision\n");
        canPlace = false;
    }
    if(ch.detectMovementCollision(ch.getQuadTreeEntities(ch.quadtreeTurret, this), this)){
        logv("5) turret movement collision\n");
        canPlace = false;
    }
    /**
    if(ch.detectMovementCollision(ch.getQuadTreeEntities(ch.quadtreeObj, this), this)){
        logv("6) obj movement collision\n");
        canPlace = false;
    }
    if(ch.detectMovementCollision(ch.getQuadTreeEntities(ch.quadtreePickUp, this), this)){
        logv("7) pickup movement collision\n");
        canPlace = false;
    }
    */
    return canPlace;
}

// activates the turret
void Turret::activateTurret() {
    activated = true;
}

void Turret::onCollision() {
    // Does nothing for now
}

void Turret::collidingProjectile(const int damage) {
    health -= damage;
}

// turret ammo pool decrements by this amount
void Turret::decrementAmmo(const int amount) {
    ammo -= amount;
}

// turret shoots, this is not yet defined
void Turret::shootTurret() {

}

 // returns true if turret has >0 ammo, false otherwise
bool Turret::ammoCheckTurret() {
    return (ammo > 0);
}

 // returns true if turret has >=1 health, false otherwise
bool Turret::healthCheckTurret() {
    return (health > 0);
}

void Turret::move(const float playerX, const float playerY,
                        const float moveX, const float moveY, CollisionHandler &ch) {
    setPosition(moveX, moveY);
    if(collisionCheckTurret(playerX, playerY, moveX, moveY, ch)) {
        texture.setAlpha(PASS_ALPHA);
    } else {
        texture.setAlpha(FAIL_ALPHA);
    }
}

void Turret::placeTurret() {
    texture.setAlpha(PLACED_ALPHA);
    boolPlaced = true;
}

bool Turret::isPlaced() {
  return boolPlaced;
}

void Turret::pickUpTurret() {
    activated = false;
    setPosition(0, 0);
    texture.setAlpha(0);
}

// checks if there are any enemies in the turret's coverage area, this is not yet defined
bool Turret::targetScanTurret() {
    return true;
}
