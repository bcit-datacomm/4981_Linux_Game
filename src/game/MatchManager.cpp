#include "MatchManager.h"

MatchManager::MatchManager() : round(0), spawnTick(0), zombiesToSpawn(0) {

}

void MatchManager::checkMatchState() {
    if (GameManager::instance()->getZombieManager().size() <= 0 && zombiesToSpawn <= 0) {
          newRound();
    }
    if (zombiesToSpawn > 0) {
        spawnZombies();
    }
}

void MatchManager::spawnZombies() {
    const int currentTime = SDL_GetTicks();
    if (currentTime < (spawnTick + ZOMBIE_SPAWN_DELAY)) {
        return;
    }
    spawnTick = currentTime;
    
    const int32_t id = GameManager::instance()->createZombie(0, 0);
    Zombie& zombie = GameManager::instance()->getZombie(id);
    CollisionHandler& ch = GameManager::instance()->getCollisionHandler();
    for (auto& pos : spawnPoints) {
        if (zombiesToSpawn > 0) {
            zombie.setPosition(pos.x, pos.y);
            if (!ch.detectMovementCollision(ch.getQuadTreeEntities(
                    ch.quadtreeMarine,&zombie),&zombie)
                    || ch.detectMovementCollision(ch.getQuadTreeEntities(ch.quadtreeZombie,&zombie),&zombie)) {
                GameManager::instance()->createZombie(pos.x, pos.y);
                --zombiesToSpawn; 
            }
        } else {
           break;
        }
    }
    GameManager::instance()->deleteZombie(id);
}

void MatchManager::setSpawnPoints(std::vector<MapPoint> points) {
    spawnPoints = points;
}

void MatchManager::newRound() {
    zombiesToSpawn = (spawnPoints.size() * ++round) * 2;
}
