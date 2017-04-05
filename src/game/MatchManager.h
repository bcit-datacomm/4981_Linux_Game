#ifndef MATCH_MANAGER_H
#define MATCH_MANAGER_H

#include <string>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sstream>
#include <iomanip>
#include <vector>
#include "GameManager.h"
#include "../map/Map.h"

static constexpr int ZOMBIE_SPAWN_DELAY = 5000;

class MatchManager {
public:

    MatchManager();
    ~MatchManager() = default;
    
    void checkMatchState();
    void newRound();
    void spawnZombies();
    void setSpawnPoints(std::vector<MapPoint> points);

private:
    
    unsigned int round;
    int spawnTick;
     unsigned int zombiesToSpawn;
    std::vector<MapPoint> spawnPoints;
    
};

#endif
