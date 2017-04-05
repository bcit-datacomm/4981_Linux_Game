#ifndef ENTITYDUMP_H
#define ENTITYDUMP_H
/*Created by Maitiu Morton 4/3/2017
 */
#include <utility>
#include <fstream>
#include <string>
#include "../basic/Entity.h"
#include "../player/Player.h"
#include "../game/GameManager.h"
#include "../game/GameHashMap.h"

void dumpEntityPositions(const Player* p);


//std::pair<float, float> getCoordinates(const Entity* e);

void printEntityPositions(std::string entityName, int32_t id, const Entity &e, std::ofstream &entitydump);
std::pair<float, float> getDestCoordinates(const Entity* e);
std::pair<float, float> getMoveCoordinates(const Entity* e);
std::pair<float, float> getSrcCoordinates(const Entity* e);
#endif
