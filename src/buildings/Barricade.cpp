/**
 * Date: Feb. 02, 2017
 * Designer: Terry Kang, Mark Tattrie
 * Programmer: Terry Kang, Mark Tattrie
 * Functions: Barricade(const int32_t nid, const SDL_Rect& dest, const SDL_Rect& movementSize,
 *              const SDL_Rect& pickupSize, const int health = 100, const int state = 0, const bool placeable = false,
 *              const bool placed = false);
 *            virtual ~Barricade();
 *            void move(const float, const float, const float, const float, CollisionHandler&); // Moves Zombie
 *            void onCollision();
 *            void collidingProjectile(const int damage);
 *            bool isPlaceable() const;
 *            bool isPlaced() const;
 *            bool checkPlaceablePosition(const float, CollisionHandler&);
 *            void placeBarricade();
 * Description:
 * Source file for the Barricade class. This file defines all the functions associated with a barricade.
 */
#include <math.h>
#include <random>
#include <cassert>

#include "Barricade.h"
#include "../game/GameManager.h"
#include "../log/log.h"

#define PI 3.14159265


/**
 * Date: Feb. 02, 2017
 *
 * Designer: Terry Kang
 * Programmer: Terry Kang
 * Function Interface: Barricade(const int32_t nid, const SDL_Rect& dest, const SDL_Rect& movementSize,
 *      const SDL_Rect& pickupSize, const int health, const int state, const bool placeable, const bool placed)
 * Description:
 * Constructor for a barricade, creates a barricade on the map.
 */
Barricade::Barricade(const int32_t nid, const SDL_Rect& dest, const SDL_Rect& movementSize, const SDL_Rect& pickupSize,
        const int health, const int state, const bool placeable, const bool placed): Object(nid, dest,
        movementSize, pickupSize, BARRICADE_HEIGHT, BARRICADE_WIDTH),health(health), state(state),
        placeable(placeable), placed(placed) {
    logv("Create Barricade\n");

}

/**
 * Date: Feb. 02, 2017
 *
 * Designer: Terry Kang
 * Programmer: Terry Kang
 * Function Interface: ~Barricade()
 * Description:
 * Destructor for a barricade, destorys a barricade on the map.
 */
Barricade::~Barricade() {
    logv("Destroy Barricade\n");
}

/**
 * Date: Feb 2, 2017
 * Modified: Mar. 15 2017 - Mark Tattrie
 * Modified: Apr. 02 2017 - Terry Kang
 *      Changed the parameters
 * Author: Terry Kang
 * Function Interface: bool Barricade::checkPlaceablePosition(const float playerX, const float playerY,
 *       const float moveX, const float moveY, CollisionHandler& ch)
 * Description:
 * checks the proposed location of the barricade placement to see if it is valid.
 */
bool Barricade::checkPlaceablePosition(const float distance, CollisionHandler& ch) {
    placeable = (distance <= 200);

    if(placeable && (ch.detectMovementCollision(ch.getQuadTreeEntities(ch.getMarineTree(),this),this)
            || ch.detectMovementCollision(ch.getQuadTreeEntities(ch.getZombieTree(),this),this)
            || ch.detectMovementCollision(ch.getQuadTreeEntities(ch.getBarricadeTree(),this),this)
            || ch.detectMovementCollision(ch.getQuadTreeEntities(ch.getWallTree(),this),this)
            || ch.detectMovementCollision(ch.getQuadTreeEntities(ch.getTurretTree(),this),this)
            || ch.detectMovementCollision(ch.getQuadTreeEntities(ch.getObjTree(),this),this))) {
        placeable = false;
    }
    return placeable;
}

/**
 * Date: Feb. 02, 2017
 *
 * Designer: Terry Kang
 * Programmer: Terry Kang
 * Function Interface: bool isPlaceable() const
 * Description:
 * returns whether or not the barricade is able to be placed
 */
bool Barricade::isPlaceable() const {
    return placeable;
}

/**
 * Date: Feb. 02, 2017
 *
 * Designer: Terry Kang
 * Programmer: Terry Kang
 * Function Interface: bool isPlaced() const
 * Description:
 * returns whether or not the barricade is placed
 */
bool Barricade::isPlaced() const {
    return placed;
}

/**
 * Date: Feb. 02, 2017
 *
 * Designer: Terry Kang
 * Programmer: Terry Kang
 * Function Interface: void move(const float playerX, const float playerY,
 *          const float moveX, const float moveY, CollisionHandler& ch)
 * Description:
 * Move Zombie by x and y amount
 */
void Barricade::move(const float playerX, const float playerY, const float moveX,
        const float moveY, CollisionHandler& ch) {
    const float distanceX = (playerX - moveX) * (playerX - moveX);
    const float distanceY = (playerY - moveY) * (playerY - moveY);
    const float distance = sqrt(abs(distanceX+distanceY));

    checkPlaceablePosition(distance, ch);
    setPosition(moveX, moveY);
}

/**
 * Date: Feb. 02, 2017
 *
 * Designer: Terry Kang
 * Programmer: Terry Kang
 * Function Interface: void onCollision()
 * Description:
 * Called when collision occurs
 */
void Barricade::onCollision() {
    // Do nothing for now
}

/**
 * Date: Feb. 02, 2017
 * Modified: March 30, 2017 - Mark Tattrie
 * Designer: Terry Kang
 * Programmer: Terry Kang
 * Function Interface: void collidingProjectile(const int damage)
 * Description:
 * Called when collision with projectile occurs
 */
void Barricade::collidingProjectile(const int damage) {
    health -= damage;
    if(health <= 0){
        GameManager::instance()->deleteBarricade(getId());
    }
}

/**
 * Date: Feb. 02, 2017
 *
 * Designer: Terry Kang
 * Programmer: Terry Kang
 * Function Interface: void placeBarricade()
 * Description:
 * Set the flag when barricade placed
 */
void Barricade::placeBarricade() {
    // texture.setAlpha(255);
    placed = true;
}
