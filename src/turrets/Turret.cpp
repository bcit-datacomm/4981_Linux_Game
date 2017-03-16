// Created 05/02/2017 Mark C.
#include "Turret.h"
#include "../game/GameManager.h"
#include "../log/log.h"

Turret::Turret(bool activated, int health, int ammo, bool placed, float range)
        :Movable(MARINE_VELOCITY), activated(activated), ammo(ammo), boolPlaced(placed), range(range) {
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
    HitBox hitBox(moveX, moveY, checkBox);
    const float distanceX = (playerX - moveX) * (playerX - moveX);
    const float distanceY = (playerY - moveY) * (playerY - moveY);
    const float distance = sqrt(abs(distanceX + distanceY));

    return (distance <= PLACE_DISTANCE && !ch.detectMovementCollision(this));
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

/**
 * Checks if there are any enemies in the turret's coverage area.
 * Jamie, March 1
 * Revised by Rob, March 5
 */
bool Turret::targetScanTurret() {
    //Get map of all zombies
    const auto& mapZombies = GameManager::instance()->getZombies();

    unsigned int closestZombieId = 0;
    float closestZombieDist = std::numeric_limits<float>::max();

    // Detect zombies
    bool detect = false;
    for (const auto& item : mapZombies) {
        const auto& zombie = item.second;
        const float zombieX = zombie.getX();
        const float zombieY = zombie.getY();

        const float xDelta = abs((abs(zombieX - ZOMBIE_WIDTH / 2) - abs(getX() - TURRET_WIDTH / 2)));
        const float yDelta = abs((abs(zombieY - ZOMBIE_HEIGHT / 2) - abs(getY() - TURRET_HEIGHT / 2)));
        const float distance = sqrt(xDelta * xDelta + yDelta * yDelta);

        if (distance < getRange()) {
            if (distance < closestZombieDist) {
                closestZombieId = item.first;
                closestZombieDist = distance;
                detect = true;
            }
        }
    }

    if (!detect) {
        return false;
    }

    const auto& target = mapZombies.find(closestZombieId);
    if (target == mapZombies.end()) {
        return false;
    }

    const float deltaX = getX() - target->second.getX();
    const float deltaY = getY() - target->second.getY();

    // Set angle so turret points at zombie
    setAngle(((atan2(deltaX, deltaY) * 180.0) / M_PI) * -1);
    //detectList[closestZombieId]->damage(this->attackDmg);

    return true;
}
