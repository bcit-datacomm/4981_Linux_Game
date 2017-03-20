#include "Object.h"
#include "../log/log.h"

Object::Object(int32_t nid, const SDL_Rect dest, int width, int height, double angle):
        Entity(nid, dest), width(width), height(height), angle(angle){
    setHeight(height);
    setWidth(width);
    updateRectHitBoxes();
}


Object::Object(int32_t nid, const SDL_Rect dest, const SDL_Rect &movementSize, const SDL_Rect &pickupSize,
        int width, int height, double angle): Entity(nid, dest,movementSize, pickupSize), width(width),
        height(height), angle(angle){
    setHeight(height);
    setWidth(width);
    updateRectHitBoxes();
}

Object::~Object() {
//    logv("Destroy Object\n");
}

void Object::setHeight(const int h) {
    height = h;
}
void Object::setWidth(const int w) {
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
void Object::setAngle(const double a){
    angle = a;
}

//returns the angle of the object's sprite
double Object::getAngle(){
    return angle;
}
