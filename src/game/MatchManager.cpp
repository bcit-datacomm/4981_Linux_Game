#include "MatchManager.h"

/**
 * Date: Apl. 4, 2017
 * Author: Jacob McPhail
 * Function Interface: MatchManager()
 * Description:
 *      ctor for the match manager.
 */
MatchManager::MatchManager() : round(0), spawnTick(0), zombiesToSpawn(0) {

}

/**
 * Date: Apl. 4, 2017
 * Author: Jacob McPhail
 * Function Interface: checkMatchState()
 * Description:
 *      check state of match, if no zombies start new round, if not spawn any unspawn zombies.
 */
void MatchManager::checkMatchState() {
    if (GameManager::instance()->getZombieManager().size() <= 0 && zombiesToSpawn <= 0) {
          newRound();
    }
    if (zombiesToSpawn > 0) {
        spawnZombies();
    }
}

/**
 * Date: Apl. 4, 2017
 * Author: Jacob McPhail.
 * Function Interface: spawnZombies()
 * Description:
 *      Spawns zombies at spawn points.
 */
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

/**
 * Date: Apl. 4, 2017
 * Author: Jacob McPhail.
 * Function Interface: setSpawnPoints(std::vector<MapPoint> points)
 *      points : Map zombie spawn points
 *
 * Description:
 *      Copy zombie spawn points from map.
 */
void MatchManager::setSpawnPoints(std::vector<MapPoint> points) {
    spawnPoints = points;
}

/**
 * Date: Apl. 4, 2017
 * Author: Jacob McPhail.
 * Function Interface: newRound()
 * Description:
 *      Starts a new round, adds zombies to spawn.
 */
void MatchManager::newRound() {
    zombiesToSpawn = (spawnPoints.size() * ++round) * 2;
}
