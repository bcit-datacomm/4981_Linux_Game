#include "Wall.h"
#include "../log/log.h"

Wall::Wall(int height, int width) : Object(height, width) {
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

