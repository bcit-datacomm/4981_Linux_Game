#include "Marine.h"
#include "../game/GameManager.h"
#include "../log/log.h"

Marine::Marine(const int32_t id, const SDL_Rect& dest, const SDL_Rect& movementSize, const SDL_Rect& projectileSize,
        const SDL_Rect& damageSize): Entity(id, dest, movementSize, projectileSize, damageSize),
        Movable(id, dest, movementSize, projectileSize, damageSize, MARINE_VELOCITY) {
    //movementHitBox.setFriendly(true); Uncomment to allow movement through other players
    //projectileHitBox.setFriendly(true); Uncomment for no friendly fire
    //damageHitBox.setFriendly(true); Uncomment for no friendly fire
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

void Marine::activateStore(Entity *ep){
    GameManager *gm = GameManager::instance();
    if(gm->storeExists(ep->getId())){
        gm->getStore(ep->getId())->purchase(2);
    }
}
