#include "Zombie.h"
#include <math.h>
#include <random>
#include <utility>
#include "Astar.h"
using namespace std;

#define PI 3.14159265
#define MAP_HEIGHT 2000
#define MAP_WIDTH 2000

Zombie::Zombie()  {
	zombieSpriteClips[0].x = 0;
	zombieSpriteClips[0].y = 0;
	zombieSpriteClips[0].w = 100;
	zombieSpriteClips[0].h = 100;
	this->movementHitBox.setRect(zombieSpriteClips[0]);
	this->movementHitBox.move(this->getX(), this->getY());
	this->movementHitBox.attached = this;
	this->projectileHitBox.setRect(zombieSpriteClips[0]);
	this->projectileHitBox.move(this->getX(), this->getY());
	this->projectileHitBox.attached = this;
	this->damageHitBox.setRect(zombieSpriteClips[0]);
	this->damageHitBox.move(this->getX(), this->getY());
	this->damageHitBox.attached = this;

	//this->setAngle(this->getRandomAngle());

	path = buildCoordinatePath(getX(), getY(), MAP_HEIGHT/2, MAP_WIDTH/2);

	printf("Create Zombie\n");
}

Zombie::~Zombie() {
	printf("Destory Zombie\n");
}

// Move Zombie by x and y amount
void Zombie::move(float moveX, float moveY, CollisionHandler* ch) {
	//Move the Zombie left or right
	this->setX(this->getX()+moveX);
	this->movementHitBox.move(this->getX(), this->getY());
	this->projectileHitBox.move(this->getX(), this->getY());

	if (ch->detectMovementCollision(&this->movementHitBox)) {
		this->setX(this->getX()-moveX);
		this->movementHitBox.move(this->getX(), this->getY());
		this->projectileHitBox.move(this->getX(), this->getY());
	}

	//Move the Zombie up or down
	this->setY(this->getY()+moveY);
	this->movementHitBox.move(this->getX(), this->getY());
	this->projectileHitBox.move(this->getX(), this->getY());

	if (ch->detectMovementCollision(&this->movementHitBox)) {
		this->setY(this->getY()-moveY);
		this->movementHitBox.move(this->getX(), this->getY());
		this->projectileHitBox.move(this->getX(), this->getY());
	}

}

// Set delta x coordinate
void Zombie::setDX(float px) {
	dx = px;
}

// Set delta y coordinate
void Zombie::setDY(float py) {
	dy = py;
}

// set velocity of Zombie movement
void Zombie::setVelocity(int pvel) {
	velocity = pvel;
}

// Get delta x coordinate
float Zombie::getDX() {
	return dx;
}

// Get delta y coordinate
float Zombie::getDY() {
	return dy;
}

// Get velocity of Zombie movement
int Zombie::getVelocity() {
	return velocity;
}

void Zombie::onCollision() {
	// Do nothing for now
}

void Zombie::collidingProjectile(int damage) {
	this->health = health - damage;
}

//sets the angle of zombie's sprite
void Zombie::setAngle(double a){
    angle = a;
}

//returns the angle of the player's marine sprite
double Zombie::getAngle(){
    return angle;
}

void Zombie::generateMove(){
	bool goingUp;
	bool goingRight;

	pair<int, int> nextStep = path.front();
	path.erase(path.begin());

	int x = nextStep.first;
	int y = nextStep.second;

	this->setDX(getVelocity() + (x + getX()));
	this->setDY(getVelocity() + (y + getY()));


}
/*
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
*/
