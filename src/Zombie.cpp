#include "Zombie.h"
#include <math.h>
#include <random>
#define PI 3.14159265
#define ZOMBIE_VELOCITY 200

Zombie::Zombie() : Movable(ZOMBIE_VELOCITY) {
	zombieSpriteClips[0].x = 0;
	zombieSpriteClips[0].y = 0;
	zombieSpriteClips[0].w = ZOMBIE_WIDTH;
	zombieSpriteClips[0].h = ZOMBIE_HEIGHT;
	this->movementHitBox.setRect(zombieSpriteClips[0]);
	this->movementHitBox.move(this->getX(), this->getY());
	this->movementHitBox.attached = this;
	this->projectileHitBox.setRect(zombieSpriteClips[0]);
	this->projectileHitBox.move(this->getX(), this->getY());
	this->projectileHitBox.attached = this;
	this->damageHitBox.setRect(zombieSpriteClips[0]);
	this->damageHitBox.move(this->getX(), this->getY());
	this->damageHitBox.attached = this;
	this->setAngle(this->getRandomAngle());
	printf("Create Zombie\n");
}

Zombie::~Zombie() {
	printf("Destory Zombie\n");
}

void Zombie::onCollision() {
	// Do nothing for now
}

void Zombie::collidingProjectile(int damage) {
	this->health = health - damage;
}

int Zombie::getRandomAngle(){
	//random number generator
	std::random_device rd;
	std::mt19937 eng(rd());

	//range 0 to 360 degrees for zombie's angle
	std::uniform_int_distribution<> distr(0,360);

	return distr(eng);
}

void Zombie::generateRandomMove(){
	int tx = this->getX()+50;
	int ty = this->getY()+50;
	if(tx<=0 || tx>=2000 || ty<=0 || ty>=2000){
		this->setAngle(this->getRandomAngle());
	}

	double cosVal = cos(this->getAngle()*PI/180.0);
	double sinVal = sin(this->getAngle()*PI/180.0);

	int x = this->getVelocity();
	int y = this->getVelocity();

	this->setDX(x*cosVal);
	this->setDY(y*sinVal);
}
