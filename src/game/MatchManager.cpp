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

void MatchManager::spawnZombies()
{
    int currentTime = SDL_GetTicks();
    if(currentTime < (spawnTick + ZOMBIE_SPAWN_DELAY)){
        return;
    }
    spawnTick = currentTime;
    
    for (auto& pos : spawnPoints) {
        if (zombiesToSpawn > 0) {
            int32_t id = GameManager::instance()->createZombie(pos.x, pos.y);
            Zombie& zombie = GameManager::instance()->getZombie(id);
            CollisionHandler& ch = GameManager::instance()->getCollisionHandler();
            if (ch.detectMovementCollision(ch.getQuadTreeEntities(
                    ch.quadtreeMarine,&zombie),&zombie)
                    || ch.detectMovementCollision(ch.getQuadTreeEntities(ch.quadtreeZombie,&zombie),&zombie)
                    || ch.detectMovementCollision(ch.getQuadTreeEntities(ch.quadtreeWall,&zombie),&zombie)
                    || ch.detectMovementCollision(ch.getQuadTreeEntities(ch.quadtreeBarricade,&zombie),&zombie)
                    || ch.detectMovementCollision(ch.getQuadTreeEntities(ch.quadtreeTurret,&zombie),&zombie)
                    || ch.detectMovementCollision(ch.getQuadTreeEntities(ch.quadtreeObj,&zombie),&zombie)
                    || ch.detectMovementCollision(ch.getQuadTreeEntities(ch.quadtreeStore,&zombie),&zombie)) {
                GameManager::instance()->deleteZombie(id);   
            } else {
                --zombiesToSpawn; 
            }
        } else {
           break;
        }
    }
}

void MatchManager::setSpawnPoints(std::vector<MapPoint> points) {
       spawnPoints.swap(points);
}

void MatchManager::newRound() {
    round++;
    zombiesToSpawn = (spawnPoints.size() * round) * 2;
}
