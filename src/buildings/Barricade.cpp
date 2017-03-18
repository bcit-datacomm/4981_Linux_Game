#include "Barricade.h"
#include "../game/GameManager.h"
#include <math.h>
#include <random>
#include "../log/log.h"
#include <cassert>
#define PI 3.14159265


Barricade::Barricade(int32_t nid, const SDL_Rect &dest, const SDL_Rect &movementSize, const SDL_Rect &pickupSize,
        int health, int state, bool placeable, bool placed): Object(nid, dest, movementSize, pickupSize,
        BARRICADE_HEIGHT, BARRICADE_WIDTH),health(health), state(state), placeable(placeable), placed(placed) {
    logv("Create Barricade\n");

}

Barricade::~Barricade() {
    logv("Destroy Barricade\n");
}

bool Barricade::checkPlaceablePosition(const float playerX, const float playerY,
        const float moveX, const float moveY, CollisionHandler  &ch) {
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

bool Barricade::isPlaceable() {
    return placeable;
}

bool Barricade::isPlaced() {
    return placed;
}

// Move Zombie by x and y amount
void Barricade::move(const float playerX, const float playerY, const float moveX,
        const float moveY, CollisionHandler &ch) {
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
    placed=true;
}
