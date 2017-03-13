// Created 05/02/2017 Mark C.
#include <map>
#include <limits>
#include "GameManager.h"

#include "Turret.h"

Turret::Turret() : Movable(MARINE_VELOCITY) {
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

// checks if the turret placement overlaps with any currently existing objects, currently does nothing
bool Turret::collisionCheckTurret(float x, float y, CollisionHandler& ch) {
    SDL_Rect checkBox;
    checkBox.h = 100;
    checkBox.w = 100;
    checkBox.x = x;
    checkBox.y = y;
    HitBox hitBox(x, y, checkBox, nullptr);
    if(ch.detectMovementCollision(&hitBox))
        return false;
    return true;
}

// activates the turret
void Turret::activateTurret() {
    activated = true;
}

void Turret::onCollision() {
    // Does nothing for now
}

void Turret::collidingProjectile(int damage) {
    health = health - damage;
}

// turret ammo pool decrements by this amount
void Turret::decrementAmmo(int amount) {
    ammo = ammo- amount;
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

/**
 * Checks if there are any enemies in the turret's coverage area.
 * The coverage area is not explicitly defined yet, feel free to change.
 * Jamie, March 1
 * Revised by Rob, March 5
 */
bool Turret::targetScanTurret() {
    //Get map of all zombies
    const std::map<unsigned int, Zombie>& mapZombies = GameManager::instance()->getZombies();

    unsigned int closestZombieId = 0;
    float closestZombieDist = std::numeric_limits<float>::max();

    // Detect zombies;
    // TODO: Improve the algorithm that selects zombie(s) to attack.
    bool bDetect = false;
    for (const auto& item : mapZombies) {
        const auto& zombie = item.second;
        float zombieX = zombie.getX();
        float zombieY = zombie.getY();

        float xDelta = zombieX - getX();
        float yDelta = zombieY - getY();
        float sqrDist = xDelta * xDelta + yDelta * yDelta;
        float sqrRange = getRange() * getRange();

        if (sqrDist < sqrRange) {
            if (sqrDist < closestZombieDist) {
                closestZombieId = item.first;
                closestZombieDist = sqrDist;
                bDetect = true;
            }
        }
    }

    // TODO: Attack a zombie;
    if (!bDetect) {
        return false;
    }

    const auto& target = mapZombies.find(closestZombieId);
    if (target == mapZombies.end()) {
        return false;
    }

    float deltaX = getX() - target->second.getX();
    float deltaY = getY() - target->second.getY();

    // Set angle so turret points at zombie
    double angle = ((atan2(deltaX, deltaY) * 180.0)/M_PI) * - 1;
    setAngle(angle);
     //detectList[closestZombieId]->damage(this->attackDmg);

    return true;
}

// returns the turret's range.
// Jamie, 2017-03-01.
inline float Turret::getRange() const
{
    return this->range;
}
