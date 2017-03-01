 #include "Marine.h"

Marine::Marine() : Movable(MARINE_VELOCITY) {
	//this->movementHitBox.setFriendly(true); Uncomment to allow movement through other players
	//this->projectileHitBox.setFriendly(true); Uncomment for no friendly fire
	//this->damageHitBox.setFriendly(true); Uncomment for no friendly fire
}

Marine::~Marine() {
	printf("Destroy Marine\n");
}

void Marine::onCollision() {
	// Do nothing for now
}

void Marine::collidingProjectile(int damage) {
	this->health = health - damage;
}
