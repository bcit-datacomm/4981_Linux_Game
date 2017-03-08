#ifndef SERVERGAMESTATE_H
#define SERVERGAMESTATE_H

#include <vector>
#include <unordered_map>
#include <utility>
#include "../GameManager.h"
#include "../UDPHeaders.h"
#include "server.h"

extern GameManager *gm;
extern std::unordered_map<unsigned int, MoveAction> moveMap;
extern std::unordered_map<unsigned int, AttackAction> attackMap;

void updateMarine(const MoveAction& ma);
void saveMoveAction(const unsigned int& id, const MoveAction& ma);
void saveAttack(const unsigned int& id, const AttackAction& aa);
void clearMoveActions();
void clearAttackActions();
void startGame();

std::vector<PlayerData> getPlayers();
std::vector<MoveAction> getMoveActions(const unsigned int& id);
std::vector<AttackAction> getAttackActions(const unsigned int& id);
std::vector<ZombieData> getZombies();

#endif
