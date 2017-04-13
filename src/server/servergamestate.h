/*------------------------------------------------------------------------------
* Header: servergamestate.h
*
* Functions:
* void updateMarine(const MoveAction& ma);
* void performAttack(const AttackAction& aa);
* void processBarricade(const BarricadeAction& ba);
* void processTurret(const TurretAction& ta);
* void deleteEntity(const DeleteAction& da);
* void saveDeletion(const DeleteAction& da);
* void saveAttack(const AttackAction& aa);
* void clearAttackActions();
* void clearDeleteActions();
* void startGame();
* std::vector<PlayerData> getPlayers();
* std::vector<ZombieData> getZombies();
* std::vector<DeleteAction> getDeletions();
*
* Date: Apr 12th 2017
*
* Designer(s): John Agapeyev
*
* Author(s): John Agapeyev
*
* Notes:
* The header for game state integration calls the server makes during the game.
* These calls are used to modify game state based on in-game results, or received
* packets.
* ------------------------------------------------------------------------------*/

#ifndef SERVERGAMESTATE_H
#define SERVERGAMESTATE_H

#include <vector>
#include <utility>
#include "../game/GameManager.h"
#include "../UDPHeaders.h"
#include "server.h"

extern GameManager *gm;
extern std::vector<AttackAction> attackList;
extern std::vector<DeleteAction> deleteList;

void updateMarine(const MoveAction& ma);
void performAttack(const AttackAction& aa);
void processBarricade(const BarricadeAction& ba);
void processTurret(const TurretAction& ta);
void deleteEntity(const DeleteAction& da);
void saveDeletion(const DeleteAction& da);
void saveAttack(const AttackAction& aa);
void clearAttackActions();
void clearDeleteActions();
void startGame();

std::vector<PlayerData> getPlayers();
std::vector<ZombieData> getZombies();
std::vector<DeleteAction> getDeletions();

#endif
