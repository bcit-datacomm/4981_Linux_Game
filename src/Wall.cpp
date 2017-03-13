#include "Wall.h"

Wall::Wall(int height, int width) : Object(height, width) {
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

