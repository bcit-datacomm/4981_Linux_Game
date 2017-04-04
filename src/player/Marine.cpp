#include "Marine.h"
#include <cstdlib>
#include "../game/GameManager.h"
#include "../log/log.h"

Marine::Marine(const int32_t id, const SDL_Rect& dest, const SDL_Rect& movementSize,
        const SDL_Rect& projectileSize, const SDL_Rect& damageSize)
: Entity(id, dest, movementSize, projectileSize, damageSize),
        Movable(id, dest, movementSize, projectileSize, damageSize, MARINE_VELOCITY) {
    logv("Create Marine\n");
}

Marine::~Marine() {
    logv("Destroy Marine\n");
}

void Marine::onCollision() {
    // Do nothing for now
}

void Marine::collidingProjectile(const int damage) {
    health -= damage;
    /**
    if (health <= 0){
        GameManager::instance()->deleteMarine(getId());
    }
    */
}

// Created by DericM 3/8/2017
void Marine::fireWeapon() {
    Weapon *w = inventory.getCurrent();
    if(w) {
        w->fire(*this);
    } else {
        logv("Slot Empty\n");
    }
}


/*
 * Created By Maitiu
 * Modified: Mar. 15 2017 - Mark Tattrie
 * Description: Checks The pick up Hitboxes of the Weapon Drops and Turrets to see if the player's
 * Marine is touching them IF Touching a Weapon Drop it Calls the Inventory Pick up method.
 */
int32_t Marine::checkForPickUp() {
    int32_t pickId = -1;
    GameManager *gm = GameManager::instance();
    CollisionHandler& ch = gm->getCollisionHandler();

    Entity *ep = ch.detectPickUpCollision(ch.getQuadTreeEntities(ch.quadtreeStore,this),this);
    if(ep){
        activateStore(ep);
        return -1;
    }
    ep = ch.detectPickUpCollision(ch.getQuadTreeEntities(ch.quadtreePickUp,this),this);
    if(ep) {
        //get Entity drop Id
        pickId = ep->getId();
        logv("Searching for id:%d in weaponDropManager\n", pickId);
        // checks if Id matches any turret Ids in turretManager, if yes, then return with the Id
        if (gm->getTurretManager().count(pickId)) {
            return pickId;
        }
        //Checks if WeaponDrop exists
        if(gm->weaponDropExists(pickId)) {
            const WeaponDrop& wd = gm->getWeaponDrop(pickId);
            //Get Weaopn id from weapon drop
            pickId = wd.getWeaponId();
            //Picks up Weapon
            if(inventory.pickUp(pickId, wd.getX(), wd.getY())) {
                int32_t DropPoint = wd.getDropPoint();
                if(DropPoint != -1){
                    gm->freeDropPoint(DropPoint);
                }
                gm->deleteWeaponDrop(wd.getId());
            }
        } else {
            logv("unable to find id:%d in weaponDropManager\n", pickId);
        }
    } else {
        loge("Pick id was nullptr\n");
    }
    return -1;
}

/**
 * Date: Mar 28, 2017
 * Author: Mark Tattrie
 * Function Interface:
 * Description: Return marine's health
 */
int Marine::getHealth(){
    return health;
}

/*
 *Create by Maitiu March 30
 * Takes in an Entity that is a store and attempts a purchase
 */
void Marine::activateStore(const Entity *ep){
    GameManager *gm = GameManager::instance();
    if(gm->storeExists(ep->getId())){
        int r = rand()% 2 + 1;//random number temp for testing

        gm->getStore(ep->getId())->purchase(r);
    }
}
