#include <cstdio>
#include <unordered_map>
#include <utility>

#include "../game/Game.h"
#include "../player/Marine.h"
#include "../creeps/Zombie.h"
#include "../game/GameManager.h"
#include "servergamestate.h"

GameManager *gm = GameManager::GameManager::instance();
std::vector<AttackAction> attackList;
std::vector<DeleteAction> deleteList;

/**
 * Saves a attack action in the vector.
 * This vector stores every attack action received since the last sync packet was sent.
 * John Agapeyev March 19
 */
void saveAttack(const AttackAction& aa) {
    attackList.push_back(aa);
}

/**
 * Clears all the stored attack actions.
 * John Agapeyev March 19
 */
void clearAttackActions() {
    attackList.clear();
}

void clearDeleteActions() {
    deleteList.clear();
}

/**
 * Updates a player marine based on a given move action struct.
 * John Agapeyev March 19
 */
void updateMarine(const MoveAction& ma) {
    std::lock_guard<std::mutex> lock(mut);
    if (gm->hasMarine(ma.id)) {
        const auto& p = gm->getMarine(ma.id);
        if (!p.second) {
            logv("Marine not found with id %d\n", ma.id);
            return;
        }
        auto& marine = p.first;
        marine.setPosition(ma.xpos, ma.ypos);
        marine.setDX(ma.xdel);
        marine.setDY(ma.ydel);
        marine.setVelocity(ma.vel);
        marine.setAngle(ma.direction);
    } else {
        logv("Marine not found with id %d\n", ma.id);
    }
}

void performAttack(const AttackAction& aa) {
    std::lock_guard<std::mutex> lock(mut);
    if (gm->hasMarine(aa.playerid)) {
        const auto& p = gm->getMarine(aa.playerid);
        if (!p.second) {
            logv("Marine not found with id %d\n", aa.playerid);
            return;
        }
        auto& marine = p.first;
        marine.setPosition(aa.xpos, aa.ypos);
        marine.setAngle(aa.direction);

        /* Using marine.fireWeapon instead because weapon ids aren't implemented and I wanted
        to get shooting working. From Brody */
        //const auto& weapon = gm->getWeapon(aa.weaponid);
        //weapon->fire(marine);

        marine.fireWeapon();
    } else {
        logv("Marine not found with id %d\n", aa.playerid);
    }
}

void processBarricade(const BarricadeAction& ba) {
    std::lock_guard<std::mutex> lock(mut);
    Barricade& tempBarricade = GameManager::instance()->getBarricade(ba.barricadeid);
    if (ba.actionid == UDPHeaders::PICKUP) {
        //No noticeable code in game logic for picking up a barricade
    } else if (ba.actionid == UDPHeaders::DROPOFF) {
        if (tempBarricade.isPlaceable()) {
            tempBarricade.placeBarricade();
            tempBarricade.setPosition(ba.xpos, ba.ypos);
        }
    } else {
        logv("Received barricade packet with unknown action id\n");
    }
}

void processTurret(const TurretAction& ta) {
    std::lock_guard<std::mutex> lock(mut);
    Turret& tempTurret = GameManager::instance()->getTurret(ta.turretid);
    if (ta.actionid == UDPHeaders::PICKUP) {
        tempTurret.pickUpTurret();
        tempTurret.setPosition(ta.xpos, ta.ypos);
    } else if (ta.actionid == UDPHeaders::DROPOFF) {
        tempTurret.placeTurret();
        tempTurret.setPosition(ta.xpos, ta.ypos);
    } else {
        logv("Received turret packet with unknown action id\n");
    }
}

/**
 * Creates a vector of PlayerData structs for use in generating outut
 * packets.
 * The PlayerData struct is defined in UDPHeaders.h
 * John Agapeyev March 19
 */
std::vector<PlayerData> getPlayers() {
    std::vector<PlayerData> rtn;
    PlayerData tempPlayer;
    std::lock_guard<std::mutex> lock(mut);
    for (const auto& idPlayerPair : gm->getAllMarines()) {
        const auto& marine = idPlayerPair.second;
        memset(&tempPlayer, 0, sizeof(tempPlayer));

        tempPlayer.playerid = idPlayerPair.first;
        tempPlayer.xpos = marine.getX();
        tempPlayer.ypos = marine.getY();
        tempPlayer.xdel = marine.getDX();
        tempPlayer.ydel = marine.getDY();
        tempPlayer.vel = marine.getVelocity();
        tempPlayer.direction = marine.getAngle();
        tempPlayer.health = marine.getHealth();

        logv("Player ID: %d\nPlayer x: %f\nPlayer y: %f\nPlayer dx: %f\nPlayer dy: %f\nDirection: %f\n",
            tempPlayer.playerid, tempPlayer.xpos, tempPlayer.ypos, tempPlayer.xdel, tempPlayer.ydel, tempPlayer.direction);

        rtn.push_back(tempPlayer);
    }
    return rtn;
}

/**
 * Similar to the getPlayers() method defined above, except this method retrieves
 * the ZombieData struct contents for each zombie.
 * John Agapeyev March 19
 */
std::vector<ZombieData> getZombies() {
    std::vector<ZombieData> rtn;
    ZombieData tempZombie;
    std::lock_guard<std::mutex> lock(mut);
    for (const auto& idZombiePair : gm->getAllZombies()) {
        const auto& zombie = idZombiePair.second;
        memset(&tempZombie, 0, sizeof(tempZombie));

        tempZombie.zombieid = idZombiePair.first;
        tempZombie.health = zombie.getHealth();
        tempZombie.xpos = zombie.getX();
        tempZombie.ypos = zombie.getY();
        tempZombie.direction = zombie.getAngle();

        rtn.push_back(tempZombie);
    }
    return rtn;
}

std::vector<DeleteAction> getDeletions() {
    return deleteList;
}

void deleteEntity(const DeleteAction& da) {
    switch(da.entitytype) {
        case UDPHeaders::MARINE:
            gm->deleteMarine(da.entityid);
            break;
        case UDPHeaders::ZOMBIE:
            gm->deleteZombie(da.entityid);
            break;
        case UDPHeaders::TURRET:
            gm->deleteTurret(da.entityid);
            break;
        case UDPHeaders::BARRICADE:
            gm->deleteBarricade(da.entityid);
            break;
        case UDPHeaders::OBJECT:
            gm->deleteObject(da.entityid);
            break;
        case UDPHeaders::WEAPON:
            gm->removeWeapon(da.entityid);
            break;
        case UDPHeaders::WEAPONDROP:
            gm->deleteWeaponDrop(da.entityid);
            break;
        default:
            logv("Deletion packet with unknown type received\n");
            break;
    }
}

void saveDeletion(const DeleteAction& da) {
    deleteList.push_back(da);
}

/**
 * Simple wrapper to start the game from a game logic perspective
 * John Agapeyev March 19
 */
void startGame() {
    Game game;
    game.run();
}
