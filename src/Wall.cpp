#include "Wall.h"

Wall::Wall(int32_t nid, const SDL_Rect dest, const SDL_Rect &spriteSize, const SDL_Rect &movementSize,
        const SDL_Rect &pickupSize, int height, int width) : Object(nid, dest, spriteSize, movementSize,
        pickupSize, height, width) {
    printf("Create Wall\n");
}

Wall::~Wall() {
    printf("Destory Wall\n");
}
void Wall::onCollision() {
    printf("Wall collision\n");
}

void Wall::collidingProjectile(int damage) {
}
