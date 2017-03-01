#include "Object.h"

Object::Object(int w, int h) : width(w), height(h) {
	spriteClips[0].x = 0;
	spriteClips[0].y = 0;
	spriteClips[0].w = w;
	spriteClips[0].h = h;
	this->movementHitBox.setRect(spriteClips[0]);
	this->movementHitBox.move(this->getX(), this->getY());
	this->movementHitBox.attached = this;
	this->projectileHitBox.setRect(spriteClips[0]);
	this->projectileHitBox.move(this->getX(), this->getY());
	this->projectileHitBox.attached = this;
	this->damageHitBox.setRect(spriteClips[0]);
	this->damageHitBox.move(this->getX(), this->getY());
	this->damageHitBox.attached = this;
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
