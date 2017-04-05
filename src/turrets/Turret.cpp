/**
 * Source: Turret.cpp
 *
 * Functions: void placementCheckTurret()
 *            bool placementCheckTurret()
 *            bool collisionCheckTurret(const float , const float , const float , const float , CollisionHandler &)
 *            void activateTurret()
 *            void collidingProjectile(const int damage)
 *            void damageTurret(const int damage)
 *            void shootTurret()
 *            bool ammoCheckTurret()
 *            bool healthCheckTurret()
 *            bool targetScanTurret()
 *            void move(const float playerX, const float playerY,
 *                    const float moveX, const float moveY, CollisionHandler &ch)
 *            void placeTurret()
 *            bool isPlaced() {return placed;}
 *            void pickUpTurret()
 *
 * Date: Feb. 02, 2017
 *
 * Designer: Mark Chen, Jaime Lee, Terry Kang
 *
 * Author: Mark Chen, Michael Goll, Jacob McPhail, Isaac Morneau, Maitiu Morton, Mark Tattrie
 *
 * Revisions:
 * Edited By : Yiaoping Shu- Style guide
 *
 * Notes:
 * Source file for the turret class. This file defines all the functions associated with a turret.
 */
#include <cassert>

#include "Turret.h"
#include "../game/GameManager.h"
#include "../log/log.h"

/**
 * Date: Feb. 02, 2017
 * Modified: Mar. 15, 2017 - Mark Chen, Maitiu Morton, Mark Tattrie
 *           Mar. 16, 2016 - Mark Chen, Michael Goll
 *
 * Designer: Mark Chen
 * Programmer: Mark Chen, Michael Goll, Maitiu Morton
 * Function Interface: Turret(int32_t id, const SDL_Rect dest, const SDL_Rect &movementSize,
         const SDL_Rect &projectileSize, const SDL_Rect &damageSize, const SDL_Rect &pickupSize,
         bool activated, int health, int ammo, bool placed)
 * Description:
 * Constructor for a turret, creates a turret on the map.
 */

Turret::Turret(const int32_t id, const SDL_Rect& dest, const SDL_Rect& movementSize, const SDL_Rect& projectileSize,
        const SDL_Rect& damageSize, const SDL_Rect& pickupSize, const bool activated, const int health,
        const int ammo, const bool placed, const float range): Entity(id, dest, movementSize,
        projectileSize, damageSize, pickupSize), Movable(id, dest, movementSize, projectileSize,
        damageSize, pickupSize, MARINE_VELOCITY), activated(activated), ammo(ammo), placed(placed),
        range(range) {
    //movementHitBox.setFriendly(true); Uncomment to allow movement through other players
    //projectileHitBox.setFriendly(true); Uncomment for no friendly fire
    //damageHitBox.setFriendly(true); Uncomment for no friendly fire
    logv("Turret created\n");
}

/**
 * Date: Feb. 02, 2017
 * Designer: Mark Chen
 * Programmer: Mark Chen
 * Function Interface: ~Turret()
 * Description:
 * Destructor for the turret object, destroys a turret from the map.
 */

Turret::~Turret() {
    logv("Destroy Turret\n");
}

/**
 * Date: Feb. 02, 2017
 * Designer: Mark Chen
 * Programmer: Mark Chen
 * Function Interface: bool placementCheckTurret()
 * Description:
 * Checks if the turret is placed within the bounds of the map. The function will return true if the
 * turret is within bounds and false if it falls outside the bounds.
 */
bool Turret::placementCheckTurret(){
    return true;
}

/**
 * Date: Mar. 10, 2017
 * Modified: Mar. 15, 2017 - Mark Chen, Mark Tattrie
 *           Mar. 16, 2017 - Mark Chen
 * Designer: Mark Chen, Terry Kang
 * Programmer: Mark Chen, Mark Tattrie
 * Function Interface: bool collisionCheckTurret(const float playerX, const float playerY, const float moveX,
         const float moveY, CollisionHandler &ch)
 * Description:
 * Checks whether the area where the user selects to place the turret is valid. The following are the two
 * requirements:
 * 1. distance must be within 200 pixels of where player is currently located
 * 2. no existing objects with a hitbox must be on the location selected
 *
 * If both requirements are met, then the function returns true. If either or both the requirements are false,
 * then the function returns false.
 */
bool Turret::collisionCheckTurret(const float playerX, const float playerY, const float moveX,
        const float moveY, CollisionHandler& ch) {
    SDL_Rect checkBox;

    checkBox.h = TURRET_HEIGHT;
    checkBox.w = TURRET_WIDTH;
    checkBox.x = moveX;
    checkBox.y = moveY;
    HitBox hitBox(checkBox);
    const float distanceX = (playerX - moveX) * (playerX - moveX);
    const float distanceY = (playerY - moveY) * (playerY - moveY);
    const float distance = sqrt(abs(distanceX + distanceY));

    return (distance <= 200 && (!ch.detectMovementCollision(ch.getQuadTreeEntities(ch.quadtreeMarine,this), this)
        && !ch.detectMovementCollision(ch.getQuadTreeEntities(ch.quadtreeZombie,this), this)
        && !ch.detectMovementCollision(ch.getQuadTreeEntities(ch.quadtreeBarricade,this), this)
        && !ch.detectMovementCollision(ch.getQuadTreeEntities(ch.quadtreeWall,this), this)
        && !ch.detectMovementCollision(ch.getQuadTreeEntities(ch.quadtreeTurret,this), this)
        && !ch.detectMovementCollision(ch.getQuadTreeEntities(ch.quadtreeObj,this), this)
        && !ch.detectMovementCollision(ch.getQuadTreeEntities(ch.quadtreePickUp,this), this)));
}

/**
 * Date: Feb. 02, 2017
 * Designer: Mark Chen
 * Programmer: Mark Chen
 * Function Interface: void activateTurret()
 * Description:
 * Changes the private member of the turret to true. This 'activates' the turret, allowing the turret to shoot
 * at zombies.
 */
void Turret::activateTurret() {
    activated = true;
}

/**
 * Date: Feb. 02, 2017
 * Modified: March 31, 2017 - Mark Tattrie
 * Designer: Mark Chen
 * Programmer: Mark Chen, Mark Tattrie
 * Function Interface: void collidingProjectile(const int damage)
 * Description:
 * Damages the turret by the amount of 'damage' parameter passed in.
 */
void Turret::collidingProjectile(const int damage) {
    health -= damage;
    //turret health is less then or equal to 0
    if(!healthCheckTurret()){
        GameManager::instance()->deleteTurret(getId());
    }
}

/**
 * Date: Feb. 02, 2017
 * Designer: Mark Chen
 * Programmer: Mark Chen
 * Function Interface: void shootTurret()
 * Description:
 * Makes the turret shoot in a direction.
 */
void Turret::shootTurret() {

}

/**
 * Date: Feb. 02, 2017
 * Designer: Mark Chen
 * Programmer: Mark Chen
 * Function Interface: bool ammoCheckTurret()
 * Description:
 * Checks the current amount of ammo for the turret's gun. If the current amount of
 * ammo is greater than 0, the function returns true and false otherwise.
 */
bool Turret::ammoCheckTurret() {
    // This is going to be changed soon so it is no longer a simple getter.
    return (ammo > 0);
}

/**
 * Date: Mar. 10, 2017
 * Modified: Mar. 15, 2017 - Mark Chen
 *           Mar. 16, 2017 - Mark Chen
 * Designer: Mark Chen, Terry Kang
 * Programmer: Mark Chen
 * Function Interface: void move(const float playerX, const float playerY,
         const float moveX, const float moveY, CollisionHandler &ch)
 * Description:
 * Changes the transparency of the turret object that replaces the mouse cursor when the player
 * is holding a turret. The turret is transparency is changed depending on whether the location
 * is a valid location.
 */
void Turret::move(const float playerX, const float playerY,
        const float moveX, const float moveY, CollisionHandler& ch) {

    setPosition(moveX, moveY);

    if (collisionCheckTurret(playerX, playerY, moveX, moveY, ch)) {
        //change the texture rendered for the turret here
        //left blank for now
    } else {
        //change the texture rendered for the turret here
        //left blank for now
    }
}

/**
 * Date: Feb. 02, 2017
 * Modified: Mar. 15, 2017 - Mark Chen
 * Designer: Mark Chen
 * Programmer: Mark Chen
 * Function Interface: void placeTurret()
 * Description:
 * Sets the turret onto the map by changing the transparency of the object and changes the private
 * member 'placed' of the turret to 'true'.
 */
void Turret::placeTurret() {
    placed = true;
}

/**
 * Date: Feb. 02, 2017
 * Modified: Mar. 15, 2017 - Mark Chen
 * Designer: Mark Chen
 * Programmer: Mark Chen
 * Function Interface: void pickUpTurret()
 * Description:
 * Picks up the truret from the map, temporarily removing it from the map by changing its private
 * member 'placed' to false.
 */
void Turret::pickUpTurret() {
    placed = false;
}

/**
 * Date: Mar. 01, 2017
 * Modified: Mar. 05, 2017 - Robert Arendac
 * Designer: Jamie Lee
 * Programmer: Jamie Lee, Robert Arendac
 * Function Interface: bool targetScanTurret()
 * Description:
 * Checks if there are any enemies in the turret's coverage area.
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
