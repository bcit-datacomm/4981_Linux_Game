#include "Wall.h"
#include "../log/log.h"

Wall::Wall(const int32_t nid, const SDL_Rect& dest, const SDL_Rect& movementSize,
        const SDL_Rect& pickupSize, const int height, const int width) : Object(nid, dest, movementSize,
        pickupSize, height, width) {
    logv("Create Wall\n");
}


Wall::~Wall() {
    logv("Destroy Wall\n");
}

void Wall::onCollision() {
    logv("Wall collision\n");
}

void Wall::collidingProjectile(const int damage) {
}
