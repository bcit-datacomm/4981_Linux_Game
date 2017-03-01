#include "Entity.h"

Entity::Entity() {
    spriteClips[0].x = 0;
	spriteClips[0].y = 0;
	spriteClips[0].w = 100;
	spriteClips[0].h = 100;
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

Entity::~Entity() {
	this->texture.free();
}

// Set x coordinate
void Entity::setX(float px) {
	x = px;
	this->movementHitBox.move(this->getX(), this->getY());
	this->projectileHitBox.move(this->getX(), this->getY());
	this->damageHitBox.move(this->getX(), this->getY());
}

// Set y coordinate
void Entity::setY(float py) {
	y = py;
	this->movementHitBox.move(this->getX(), this->getY());
	this->projectileHitBox.move(this->getX(), this->getY());
	this->damageHitBox.move(this->getX(), this->getY());
}

// Get x coordinate
float Entity::getX() {
	return x;
}

// Get y coordinate
float Entity::getY() {
	return y;
}

// Set Entity by x and y amount
void Entity::setPosition(float x, float y) {
	//Move the Entity left or right
	this->setX(x);

	//Move the Entity up or down
	this->setY(y);

}
