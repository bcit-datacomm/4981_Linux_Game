#include "Object.h"

Object::Object() {

}

Object::~Object() {
    printf("Destory Object\n");
}

void Object::setHeight(int h) {
    height = h;
    spriteClips[0].h = height;
}
void Object::setWidth(int w) {
    width = w;
    spriteClips[0].w = width;
}
int Object::getHeight(){
    return height;
}
int Object::getWidth(){
    return width;
}

void Object::onCollision() {
    // Do nothing for now
}

void Object::collidingProjectile(int damage) {
    //nothing for now
}
