#include "Barricade.h"
#include "GameManager.h"
#include <math.h>
#include <random>
#define PI 3.14159265

Barricade::Barricade()  {
	objectSpriteClips[0].x = 0;
	objectSpriteClips[0].y = 0;
	objectSpriteClips[0].w = 100;
	objectSpriteClips[0].h = 100;
	this->movementHitBox.setRect(objectSpriteClips[0]);
	this->movementHitBox.move(this->getX(), this->getY());
	this->movementHitBox.attached = this;
	this->projectileHitBox.setRect(objectSpriteClips[0]);
	this->projectileHitBox.move(this->getX(), this->getY());
	this->projectileHitBox.attached = this;
	this->damageHitBox.setRect(objectSpriteClips[0]);
	this->damageHitBox.move(this->getX(), this->getY());
	this->damageHitBox.attached = this;


	printf("Create Barricade\n");
}

Barricade::~Barricade() {
	printf("Destory Barricade\n");
}

bool Barricade::checkPlaceablePosition(float playerX, float playerY, float moveX, float moveY, CollisionHandler * ch){
	float distanceX = (playerX - moveX) * (playerX - moveX);
	float distanceY = (playerY - moveY) * (playerY - moveY);
	float distance = sqrt(abs(distanceX+distanceY));
	if(distance>200){
		this->boolPlaceable = false;
	}else
		this->boolPlaceable = true;
	if(this->boolPlaceable){
		if (ch->detectMovementCollision(&this->movementHitBox) || ch->detectMovementCollision(&this->movementHitBox)) {
			this->movementHitBox.move(this->getX(), this->getY());
			this->projectileHitBox.move(this->getX(), this->getY());
			this->damageHitBox.move(this->getX(), this->getY());
			this->boolPlaceable = false;
		}
	}
	return boolPlaceable;

}

bool Barricade::isPlaceable(){
	return boolPlaceable;
}

// Move Zombie by x and y amount
void Barricade::move(float playerX, float playerY, float moveX, float moveY, CollisionHandler * ch) {
	this->setPosition(moveX, moveY);
	this->movementHitBox.move(moveX, moveY);
	this->projectileHitBox.move(moveX, moveY);
	this->damageHitBox.move(moveX, moveY);
	
	if(this->checkPlaceablePosition(playerX, playerY, moveX, moveY, ch))
		this->texture.setAlpha(200);
	else
		this->texture.setAlpha(30);
}


void Barricade::onCollision() {
	// Do nothing for now
}

void Barricade::collidingProjectile(int damage) {
	this->health = health - damage;
}

//sets the angle of zombie's sprite
void Barricade::setAngle(double a){
    angle = a;
}

//returns the angle of the player's marine sprite
double Barricade::getAngle(){
    return angle;
}
