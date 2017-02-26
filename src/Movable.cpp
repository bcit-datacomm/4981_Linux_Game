#include "Movable.h"

// Move Movable by x and y amount
void Movable::move(float moveX, float moveY, CollisionHandler* ch) {
	//Move the Movable left or right
	this->setX(this->getX()+moveX);
	this->movementHitBox.move(this->getX(), this->getY());
	this->projectileHitBox.move(this->getX(), this->getY());
	this->damageHitBox.move(this->getX(), this->getY());

	if (ch->detectMovementCollision(&this->movementHitBox)) {
		this->setX(this->getX()-moveX);
		this->movementHitBox.move(this->getX(), this->getY());
		this->projectileHitBox.move(this->getX(), this->getY());
		this->damageHitBox.move(this->getX(), this->getY());
	}

	//Move the Movable up or down
	this->setY(this->getY()+moveY);
	this->movementHitBox.move(this->getX(), this->getY());
	this->projectileHitBox.move(this->getX(), this->getY());
	this->damageHitBox.move(this->getX(), this->getY());

	if (ch->detectMovementCollision(&this->movementHitBox)) {
		this->setY(this->getY()-moveY);
		this->movementHitBox.move(this->getX(), this->getY());
		this->projectileHitBox.move(this->getX(), this->getY());
		this->damageHitBox.move(this->getX(), this->getY());
	}

}

// Set delta x coordinate
void Movable::setDX(float px) {
	dx = px;
}

// Set delta y coordinate
void Movable::setDY(float py) {
	dy = py;
}

// set velocity of Movable movement
void Movable::setVelocity(int pvel) {
	velocity = pvel;
}

// Get delta x coordinate
float Movable::getDX() {
	return dx;
}

// Get delta y coordinate
float Movable::getDY() {
	return dy;
}

// Get velocity of Movable movement
int Movable::getVelocity() {
	return velocity;
}

//sets the angle of the player's Movable sprite
void Movable::setAngle(double a){
    angle = a;
}

//returns the angle of the player's Movable sprite
double Movable::getAngle(){
    return angle;
}
