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
std::unordered_map<unsigned int, MoveAction> moveMap;
std::unordered_map<unsigned int, AttackAction> attackMap;

void saveMoveAction(const unsigned int& id, const MoveAction& ma) {
    moveMap.insert(std::make_pair(id, ma));
}

void saveAttack(const unsigned int& id, const AttackAction& aa) {
    attackMap.insert(std::make_pair(id, aa));
}

void clearMoveActions() {
    moveMap.clear();
}

void clearAttackActions() {
    attackMap.clear();
}

void updateMarine(const MoveAction& ma) {
    Marine& marine = gm->getMarine(ma.id);
    marine.setPosition(ma.xpos, ma.ypos);
    marine.setVelocity(ma.vel);
    marine.setAngle(ma.direction);
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
        tempPlayer.actionid = ACTIONH;
        tempPlayer.nmoves = moveMap.count(idPlayerPair.first);
        tempPlayer.nattacks = attackMap.count(idPlayerPair.first);
        
        memmove(&tempPlayer.moves, getMoveActions(idPlayerPair.first).data(), tempPlayer.nmoves);
        memmove(&tempPlayer.attacks, getAttackActions(idPlayerPair.first).data(), tempPlayer.nattacks);

        rtn.push_back(tempPlayer);
    }
    return rtn;
}

std::vector<MoveAction> getMoveActions(const unsigned int& id) {
    const auto& moveRange = moveMap.equal_range(id);
    std::vector<MoveAction> moveList;
    for (auto it = moveRange.first; it != moveRange.second; ++it) {
        moveList.push_back(it->second);
    }
    return moveList;
}

std::vector<AttackAction> getAttackActions(const unsigned int& id) {
    const auto& attackRange = attackMap.equal_range(id);
    std::vector<AttackAction> attackList;
    for (auto it = attackRange.first; it != attackRange.second; ++it) {
        attackList.push_back(it->second);
    }
    return attackList;
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
