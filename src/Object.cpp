#include "Object.h"

Object::Object(int32_t nid, const SDL_Rect &spriteSize, int width, int height, double angle):
        Entity(nid, spriteSize), width(width), height(height), angle(angle){
    setHeight(height);
    setWidth(width);
    updateRectHitBoxes();
}


Object::Object(int32_t nid, const SDL_Rect &spriteSize, const SDL_Rect &movementSize, const SDL_Rect &pickupSize,
        int width, int height, double angle) : Entity(nid, spriteSize, movementSize, pickupSize), width(width), height(height), angle(angle){
    setHeight(height);
    setWidth(width);
    updateRectHitBoxes();
}

Object::~Object() {
//    printf("Destory Object\n");
}

void Object::setHeight(int h) {
    height = h;
}
void Object::setWidth(int w) {
    width = w;
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
