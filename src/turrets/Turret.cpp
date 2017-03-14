// Created 05/02/2017 Mark C.
#include "Turret.h"
#include "../game/GameManager.h"

Turret::Turret(bool activated, int health, int ammo, bool placed, float range)
        :Movable(MARINE_VELOCITY), activated(activated), ammo(ammo), boolPlaced(placed), range(range) {
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
    HitBox hitBox(moveX, moveY, checkBox);
    const float distanceX = (playerX - moveX) * (playerX - moveX);
    const float distanceY = (playerY - moveY) * (playerY - moveY);
    const float distance = sqrt(abs(distanceX+distanceY));

    return (distance <= 200 && !ch.detectMovementCollision(this));
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
    for (const auto& item : mapZombies)
    {
        const auto& zombie = item.second;
        float zombieX = zombie.getX();
        float zombieY = zombie.getY();

        float xDelta = abs((abs(zombieX - ZOMBIE_WIDTH / 2) - abs(getX() - TURRET_WIDTH / 2)));
        float yDelta = abs((abs(zombieY - ZOMBIE_HEIGHT / 2) - abs(getY() - TURRET_HEIGHT / 2)));
        xDelta *= xDelta;
        yDelta *= yDelta;
        float distance = sqrt(xDelta + yDelta);

        if (distance < getRange()) {
            if (distance < closestZombieDist) {
                closestZombieId = item.first;
                closestZombieDist = distance;
                detect = true;
            }
        }
    }

    // TODO: Attack a zombie;
    if (!detect) {
        return false;
    }

    const auto& target = mapZombies.find(closestZombieId);
    if (target == mapZombies.end())
    {
        return false;
    }

    float deltaX = getX() - target->second.getX();
    float deltaY = getY() - target->second.getY();

    // Set angle so turret points at zombie
    double angle = ((atan2(deltaX, deltaY) * 180.0)/M_PI) * -1;
    setAngle(angle);
    //detectList[closestZombieId]->damage(this->attackDmg);

    return true;
}

// returns the turret's range.
// Jamie, 2017-03-01.
float Turret::getRange() const {
    return range;
}
