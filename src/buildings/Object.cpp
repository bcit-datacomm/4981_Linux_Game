#include "Object.h"
#include "../log/log.h"

Object::Object(int width, int height, double angle) : Entity(), width(width), height(height), angle(angle){
    setHeight(height);
    setWidth(width);
    updateRectHitBoxes();
}

Object::~Object() {
//    logv("Destroy Object\n");
}

void Object::setHeight(const int h) {
    height = h;
    spriteClips[0].h = height;
}
void Object::setWidth(const int w) {
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

//sets the angle of object's sprite
void Object::setAngle(const double a){
    angle = a;
}

//returns the angle of the object's sprite
double Object::getAngle(){
    return angle;
}
