#include <math.h>
#include <random>
#include <cassert>

#include "Barricade.h"
#include "../game/GameManager.h"
#include "../log/log.h"

#define PI 3.14159265


Barricade::Barricade(const int32_t nid, const SDL_Rect& dest, const SDL_Rect& movementSize, const SDL_Rect& pickupSize,
        const int health, const int state, const bool placeable, const bool placed): Object(nid, dest,
        movementSize, pickupSize, BARRICADE_HEIGHT, BARRICADE_WIDTH),health(health), state(state),
        placeable(placeable), placed(placed) {
    logv("Create Barricade\n");

}

Barricade::~Barricade() {
    logv("Destroy Barricade\n");
}

/**
 * Date: Feb 2, 2017
 * Modified: Mar. 15 2017 - Mark Tattrie
 * Author: Terry Kang
 * Function Interface: bool Barricade::checkPlaceablePosition(const float playerX, const float playerY,
 *       const float moveX, const float moveY, CollisionHandler& ch)
 * Description:
 * checks the proposed location of the barricade placement to see if it is valid.
 */
bool Barricade::checkPlaceablePosition(const float playerX, const float playerY,
        const float moveX, const float moveY, CollisionHandler& ch) {

    const float distanceX = (playerX - moveX) * (playerX - moveX);
    const float distanceY = (playerY - moveY) * (playerY - moveY);
    const float distance = sqrt(abs(distanceX+distanceY));

    placeable = (distance <= 200);

    if(placeable && (ch.detectMovementCollision(ch.getQuadTreeEntities(ch.quadtreeMarine,this),this)
            || ch.detectMovementCollision(ch.getQuadTreeEntities(ch.quadtreeZombie,this),this)
            || ch.detectMovementCollision(ch.getQuadTreeEntities(ch.quadtreeBarricade,this),this)
            || ch.detectMovementCollision(ch.getQuadTreeEntities(ch.quadtreeWall,this),this)
            || ch.detectMovementCollision(ch.getQuadTreeEntities(ch.quadtreeTurret,this),this)
            || ch.detectMovementCollision(ch.getQuadTreeEntities(ch.quadtreeObj,this),this))) {
        placeable = false;
    }
    return placeable;
}

//returns whether or not the barricade is able to be placed
bool Barricade::isPlaceable() {
    return placeable;
}

//returns whether or not the barricade is placed
bool Barricade::isPlaced() {
    return placed;
}

// Move Zombie by x and y amount
void Barricade::move(const float playerX, const float playerY, const float moveX,
        const float moveY, CollisionHandler& ch) {
    setPosition(moveX, moveY);
}


void Barricade::onCollision() {
    // Do nothing for now
}

void Barricade::collidingProjectile(const int damage) {
    health -= damage;
}

void Barricade::placeBarricade() {
    // texture.setAlpha(255);
    placed = true;
}
