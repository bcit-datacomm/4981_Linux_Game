#include "Wall.h"

Wall::Wall() : Object(WALL_WIDTH, WALL_HEIGHT) {
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

