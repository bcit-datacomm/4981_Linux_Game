#include "Object.h"

Object::Object(int height, int width, double angle) : Entity(), height(height), width(width), angle(angle){
    setHeight(height);
    setWidth(width);
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

//sets the angle of object's sprite
void Object::setAngle(double a){
    angle = a;
}

//returns the angle of the object's sprite
double Object::getAngle(){
    return angle;
}
