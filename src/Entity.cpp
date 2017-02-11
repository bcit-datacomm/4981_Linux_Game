#include "Entity.h"

Entity::Entity() {
	
}

Entity::~Entity() {
	this->texture.free();	
}

// Set x coordinate
void Entity::setX(float px) {
	x = px;
} 

// Set y coordinate
void Entity::setY(float py) {
	y = py;
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
