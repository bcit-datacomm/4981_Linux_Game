 #include "Marine.h"

Marine::Marine() {
	marineSpriteClips[0].x = 0;
	marineSpriteClips[0].y = 0;
	marineSpriteClips[0].w = 100;
	marineSpriteClips[0].h = 100;
	this->movementHitBox.setRect(marineSpriteClips[0]);
	this->movementHitBox.move(this->getX(), this->getY());
	this->movementHitBox.attached = this;
	//this->movementHitBox.setFriendly(true); Uncomment to allow movement through other players
	this->projectileHitBox.setRect(marineSpriteClips[0]);
	this->projectileHitBox.move(this->getX(), this->getY());
	this->projectileHitBox.attached = this;
	//this->projectileHitBox.setFriendly(true); Uncomment for no friendly fire
	this->damageHitBox.setRect(marineSpriteClips[0]);
	this->damageHitBox.move(this->getX(), this->getY());
	this->damageHitBox.attached = this;
	//this->damageHitBox.setFriendly(true); Uncomment for no friendly fire
}

Marine::~Marine() {
	printf("Destroy Marine\n");
}

// Move Marine by x and y amount
void Marine::move(float moveX, float moveY, CollisionHandler* ch) {
	//Move the Marine left or right
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

	//Move the Marine up or down
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

// Set Marine by x and y amount
void Marine::setPosition(float x, float y) {

	Entity::setPosition(x, y);

	this->movementHitBox.move(this->getX(), this->getY());
	this->projectileHitBox.move(this->getX(), this->getY());
}

// Set x coordinate
void Marine::setX(float px) {
	Entity::setX(px);
	this->movementHitBox.move(this->getX(), this->getY());
	this->projectileHitBox.move(this->getX(), this->getY());
}

// Set y coordinate
void Marine::setY(float py) {
	Entity::setY(py);
	this->movementHitBox.move(this->getX(), this->getY());
	this->projectileHitBox.move(this->getX(), this->getY());
}

// Set delta x coordinate
void Marine::setDX(float px) {
	dx = px;
}

// Set delta y coordinate
void Marine::setDY(float py) {
	dy = py;
}

// set velocity of Marine movement
void Marine::setVelocity(int pvel) {
	velocity = pvel;
}

// Get delta x coordinate
float Marine::getDX() {
	return dx;
}

// Get delta y coordinate
float Marine::getDY() {
	return dy;
}

// Get velocity of Marine movement
int Marine::getVelocity() {
	return velocity;
}

void Marine::onCollision() {
	// Do nothing for now
}

void Marine::collidingProjectile(int damage) {
	this->health = health - damage;
}

//sets the angle of the player's marine sprite
void Marine::setAngle(double a){
    angle = a;
}

//returns the angle of the player's marine sprite
double Marine::getAngle(){
    return angle;
}
