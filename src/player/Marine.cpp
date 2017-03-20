#include "Marine.h"
#include "../game/GameManager.h"
#include "../log/log.h"

Marine::Marine(int32_t id, const SDL_Rect &dest, const SDL_Rect &movementSize, const SDL_Rect &projectileSize,
        const SDL_Rect &damageSize): Entity(id, dest, movementSize, projectileSize, damageSize),
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

void Marine::collidingProjectile(int damage) {
    health = health - damage;
}

// Created by DericM 3/8/2017
void Marine::fireWeapon() {
    Weapon* w = inventory.getCurrent();
    if( w != nullptr) {
        w->fire(*this);
    } else {
        logv("Slot Empty\n");
    }
}


int32_t Marine::checkForPickUp() {

    int32_t PickId = -1;

    CollisionHandler &ch = GameManager::instance()->getCollisionHandler();

    Entity* ep = ch.detectPickUpCollision(ch.getQuadTreeEntities(ch.quadtreePickUp,this),this);

    if(ep != nullptr) {
        const auto& tm = GameManager::instance()->getTurretManager();
        //get Entity drop Id
        PickId = ep->getId();
        // checks if Id matches any turret Ids in turretManager, if yes, then return with the Id
        const auto& it = tm.find(PickId);

        if (it != tm.end()) {
            return PickId;
        }

        const WeaponDrop &wd = GameManager::instance()->getWeaponDrop(PickId);

        //Get Weaopn id from weapon drop
        PickId = wd.getWeaponId();

        if(inventory.pickUp(PickId)) {

            GameManager::instance()->deleteWeaponDrop(wd.getId());
        }
    }
    return -1;
}
