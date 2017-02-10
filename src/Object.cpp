#include "Object.h"

Object::Object() {
	objectSpriteClips[0].x = 0;
	objectSpriteClips[0].y = 0;
	objectSpriteClips[0].w = 100;
	objectSpriteClips[0].h = 100;
	this->movementHitBox.setRect(objectSpriteClips[0]);
	this->movementHitBox.move(this->getX(), this->getY());
	this->movementHitBox.attached = this;
	//this->movementHitBox.setFriendly(true); Uncomment to allow movement through other players
	this->projectileHitBox.setRect(objectSpriteClips[0]);
	this->projectileHitBox.move(this->getX(), this->getY());
	this->projectileHitBox.attached = this;
	//this->projectileHitBox.setFriendly(true); Uncomment for no friendly fire
	this->damageHitBox.setRect(objectSpriteClips[0]);
	this->damageHitBox.move(this->getX(), this->getY());
	this->damageHitBox.attached = this;
	//this->projectileHitBox.setFriendly(true); Uncomment for no friendly fire

}

Object::~Object() {
	printf("Destory Object\n");
}

void Object::setPosition(float x, float y) {
	
	Entity::setPosition(x, y);
	
	this->movementHitBox.move(this->getX(), this->getY());
	this->projectileHitBox.move(this->getX(), this->getY());
	this->damageHitBox.move(this->getX(), this->getY());
}

// Set x coordinate
void Object::setX(float px) {
	Entity::setX(px);
	this->movementHitBox.move(this->getX(), this->getY());
	this->projectileHitBox.move(this->getX(), this->getY());
	this->damageHitBox.move(this->getX(), this->getY());
} 

// Set y coordinate
void Object::setY(float py) {
	Entity::setY(py);
	this->movementHitBox.move(this->getX(), this->getY());
	this->projectileHitBox.move(this->getX(), this->getY());
	this->damageHitBox.move(this->getX(), this->getY());
} 

void Object::setHeight(int h) {
	this->height = h;
	this->objectSpriteClips[0].h = this->height;
}
void Object::setWidth(int w) {
	this->width = w;
	this->objectSpriteClips[0].w = this->width;
}
int Object::getHeight(){
	return this->height;
}
int Object::getWidth(){
	return this->width;
}
