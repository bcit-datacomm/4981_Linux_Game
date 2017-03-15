#include <cstdio>
#include <unordered_map>
#include <utility>

#include "../Game.h"
#include "../LTimer.h"
#include "../Marine.h"
#include "../Zombie.h"
#include "../GameManager.h"
#include "servergamestate.h"

GameManager *gm = GameManager::GameManager::instance();
std::vector<AttackAction> attackList;

void saveAttack(const AttackAction& aa) {
    attackList.push_back(aa);
}

void clearAttackActions() {
    attackList.clear();
}

void updateMarine(const MoveAction& ma) {
    if (gm->hasMarine(ma.id)) {
        Marine& marine = gm->getMarine(ma.id);
        marine.setPosition(ma.xpos, ma.ypos);
        marine.setVelocity(ma.vel);
        marine.setAngle(ma.direction);
    } else {
        logv("Marine not found with id %d\n", ma.id);
    }
}

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

void startGame() {
    Game game;
    game.run();
    game.close();
}
