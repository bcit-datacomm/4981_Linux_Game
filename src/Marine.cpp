#include "Marine.h"

Marine::Marine() : Movable(MARINE_VELOCITY) {
    //movementHitBox.setFriendly(true); Uncomment to allow movement through other players
    //projectileHitBox.setFriendly(true); Uncomment for no friendly fire
    //damageHitBox.setFriendly(true); Uncomment for no friendly fire
    printf("Create Marine\n");
}

Marine::~Marine() {
    printf("Destroy Marine\n");
}

void Marine::onCollision() {
    // Do nothing for now
}

void Marine::collidingProjectile(int damage) {
    health = health - damage;
}
