#include "Marine.h"
#include "GameManager.h"

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

// Created by DericM 3/8/2017
void Marine::fireWeapon() {
    inventory.getCurrent()->fire(*this);
}


int32_t Marine::checkForPickUp(){
     int32_t id = -1;
     Entity *ep;
     GameManager *gm = GameManager::instance();

     CollisionHandler &ch = gm->getCollisionHandler();

     ep =  ch.detectPickUpCollision(this);

     if(ep != nullptr){
         id = ep->getId();
         WeaponDrop wd = gm->getWeaponDrop(id);
         id = wd.getWeaponId();
         inventory.pickUp(id);

         return 1;
     }

     return id;
 }
