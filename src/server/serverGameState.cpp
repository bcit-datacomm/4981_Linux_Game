#include <cstdio>
#include <unordered_map>
#include <utility>

#include "../game/Game.h"
#include "../basic/LTimer.h"
#include "../player/Marine.h"
#include "../creeps/Zombie.h"
#include "../game/GameManager.h"
#include "servergamestate.h"

GameManager *gm = GameManager::GameManager::instance();
std::vector<AttackAction> attackList;

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

/**
 * Updates a player marine based on a given move action struct.
 * John Agapeyev March 19
 */
void updateMarine(const MoveAction& ma) {
    if (gm->hasMarine(ma.id)) {
        Marine& marine = gm->getMarine(ma.id);
        marine.setPosition(ma.xpos, ma.ypos);
        marine.setDX(ma.xdel);
        marine.setDY(ma.ydel);
        marine.setVelocity(ma.vel);
        marine.setAngle(ma.direction);
    } else {
        logv("Marine not found with id %d\n", ma.id);
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
    for (const auto& idPlayerPair : gm->getAllMarines()) {
        const auto& marine = idPlayerPair.second;
        memset(&tempPlayer, 0, sizeof(tempPlayer));

        tempPlayer.playerid = idPlayerPair.first;
        tempPlayer.xpos = marine.getX();
        tempPlayer.ypos = marine.getY();
        tempPlayer.vel = marine.getVelocity();
        tempPlayer.direction = marine.getAngle();
        tempPlayer.health = marine.getHealth();

        logv("Player ID: %d\nPlayer x: %f\nPlayer y: %f\nDirection: %f\n", tempPlayer.playerid, tempPlayer.xpos, tempPlayer.ypos, tempPlayer.direction);
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

/**
 * Simple wrapper to start the game from a game logic perspective
 * John Agapeyev March 19
 */
void startGame() {
    Game game;
    game.run();
    game.close();
}
