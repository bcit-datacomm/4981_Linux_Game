// Created 05/02/2017 Mark C.
#include "Turret.h"

Turret::Turret() : Movable(MARINE_VELOCITY){
    //movementHitBox.setFriendly(true); Uncomment to allow movement through other players
    //projectileHitBox.setFriendly(true); Uncomment for no friendly fire
    //damageHitBox.setFriendly(true); Uncomment for no friendly fire
}

Turret::~Turret() {
    printf("Destroy Turret\n");
}

// checks if turret placement is within bounds, currently does nothing
bool Turret::placementCheckTurret(){
    return true;
}

// checks if the turret placement overlaps with any currently existing objects
bool Turret::collisionCheckTurret(float playerX, float playerY, float moveX, float moveY, CollisionHandler &ch) {
  float distanceX = (playerX - moveX) * (playerX - moveX);
	float distanceY = (playerY - moveY) * (playerY - moveY);
	float distance = sqrt(abs(distanceX+distanceY));
	if(distance>200) {
		placeable = false;
	}else
		placeable = true;
	if(placeable) {
        if(ch.detectMovementCollision(movementHitBox.get()))
	        placeable = false;
	}
	return placeable;
}

// activates the turret
void Turret::activateTurret() {
    activated = true;
}

void Turret::onCollision() {
    // Does nothing for now
}

void Turret::collidingProjectile(int damage) {
    health -= damage;
}

// turret ammo pool decrements by this amount
void Turret::decrementAmmo(int amount) {
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

void Turret::move(float playerX, float playerY, float moveX, float moveY, CollisionHandler &ch) {
    setPosition(moveX, moveY);
    if(this->collisionCheckTurret(playerX, playerY, moveX, moveY, ch))
        texture.setAlpha(200);
    else
        texture.setAlpha(30);
}

void Turret::placeTurret() {
    texture.setAlpha(255);
    placed = true;
}

bool Turret::isPlaceable() {
    return placeable;
}

// checks if there are any enemies in the turret's coverage area, this is not yet defined
bool Turret::targetScanTurret() {
    return true;
}
