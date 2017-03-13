// Created 05/02/2017 Mark C.
#include "Turret.h"

Turret::Turret(bool activated, int health, int ammo, bool placeable, bool placed)
                : Movable(MARINE_VELOCITY), activated(activated), ammo(ammo),
                placeable(placeable), placed(placed) {
    //movementHitBox.setFriendly(true); Uncomment to allow movement through other players
    //projectileHitBox.setFriendly(true); Uncomment for no friendly fire
    //damageHitBox.setFriendly(true); Uncomment for no friendly fire
    printf("Turret created\n");
}

Turret::~Turret() {
    printf("Destroy Turret\n");
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
    HitBox hitBox(moveX, moveY, checkBox, nullptr);
    float distanceX = (playerX - moveX) * (playerX - moveX);
  	float distanceY = (playerY - moveY) * (playerY - moveY);
  	float distance = sqrt(abs(distanceX+distanceY));
    
  	return (distance <= 200 && !ch.detectMovementCollision(&hitBox));
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
    if(this->collisionCheckTurret(playerX, playerY, moveX, moveY, ch)) {
        texture.setAlpha(PASS_ALPHA);
    } else {
        texture.setAlpha(FAIL_ALPHA);
    }
}

void Turret::placeTurret() {
    texture.setAlpha(PLACED_ALPHA);
    placed = true;
}

bool Turret::isPlaceable() {
  return placeable;
}

bool Turret::isPlaced() {
  return placed;
}

void Turret::pickUpTurret(const int32_t id) {
    activated = false;
    setPosition(0, 0);
    texture.setAlpha(0);
}

// checks if there are any enemies in the turret's coverage area, this is not yet defined
bool Turret::targetScanTurret() {
    return true;
}
