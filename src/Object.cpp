#include "Object.h"

Object::Object() {
	//this->movementHitBox.setFriendly(true); Uncomment to allow movement through other players
	//this->projectileHitBox.setFriendly(true); Uncomment for no friendly fire
	//this->projectileHitBox.setFriendly(true); Uncomment for no friendly fire
}

Object::~Object() {
	printf("Destory Object\n");
}

void Object::setHeight(int h) {
	this->height = h;
	this->spriteClips[0].h = this->height;
}
void Object::setWidth(int w) {
	this->width = w;
	this->spriteClips[0].w = this->width;
}
int Object::getHeight(){
	return this->height;
}
int Object::getWidth(){
	return this->width;
}

void Object::onCollision() {
	// Do nothing for now
}

void Object::collidingProjectile(int damage) {
    //nothing for now
}
