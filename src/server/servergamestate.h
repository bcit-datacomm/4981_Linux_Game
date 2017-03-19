#ifndef SERVERGAMESTATE_H
#define SERVERGAMESTATE_H

#include <vector>
#include <utility>
#include "../game/GameManager.h"
#include "../UDPHeaders.h"
#include "server.h"

extern GameManager *gm;
extern std::vector<AttackAction> attackList;

void updateMarine(const MoveAction& ma);
void saveAttack(const AttackAction& aa);
void clearAttackActions();
void startGame();

std::vector<PlayerData> getPlayers();
std::vector<ZombieData> getZombies();

#endif
