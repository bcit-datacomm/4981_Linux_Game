#include "GameManager.h"
#include "HitBox.h"
#include <memory>
#include <utility>

GameManager *GameManager::sInstance;
Weapon w;
//Returns the already existing GameManager or if there isn't one, makes
//a new one and returns it.
GameManager *GameManager::instance() {
    if (!sInstance)
        sInstance = new GameManager;
    return sInstance;
}

GameManager::GameManager():collisionHandler() {
    printf("Create GM\n");
}

GameManager::~GameManager() {

    printf("Destroy GM\n");
    marineManager.clear();
    zombieManager.clear();
    objectManager.clear();
    turretManager.clear();
    weaponDropManager.clear();
}

// Render all objects in level
void GameManager::renderObjects(SDL_Renderer* gRenderer, float camX, float camY) {
    for (const auto& m : weaponDropManager) {
        m.second.texture.render(gRenderer, m.second.getX() - camX, m.second.getY() - camY);
    }

    for (const auto& m : marineManager) {
        m.second.texture.render(gRenderer, m.second.getX() - camX, m.second.getY() - camY,
                                 NULL, m.second.getAngle());
    }

    for (const auto& o : objectManager) {
        o.second.texture.render(gRenderer, o.second.getX() - camX, o.second.getY() - camY);
    }

    for (const auto& z : zombieManager) {
        z.second.texture.render(gRenderer, z.second.getX() - camX, z.second.getY() - camY,
                                 NULL, z.second.getAngle());
    }

    for (const auto& m : turretManager) {
        m.second.texture.render(gRenderer, m.second.getX() - camX, m.second.getY() - camY,
                                 NULL, m.second.getAngle());
    }

 	for (const auto& b : barricadeManager) {
		b.second.texture.render(gRenderer, b.second.getX()-camX, b.second.getY()-camY);
	}

}

// Update marine movements. health, and actions
void GameManager::updateMarines(const float& delta) {
    for (auto& m : marineManager) {
        m.second.move((m.second.getDX()*delta), (m.second.getDY()*delta), collisionHandler);
    }
}

// Update zombie movements.
void GameManager::updateZombies(const float& delta) {
    for (auto& z : zombieManager) {
        // Check if the zombie is allowed to move
        z.second.checkMove();

        if (z.second.isMoving()) {
            z.second.generateMove();
        }

        z.second.move((z.second.getDX()*delta), (z.second.getDY()*delta), collisionHandler);
    }
}

// Update turret actions.
// Jamie, 2017-03-01.
void GameManager::updateTurrets(const float& delta) {
	static bool bi_frame = false;
	bi_frame = !bi_frame;
	if (!bi_frame) {
		return;
    }

	for (auto& t : turretManager) {
		//z.second->generateRandomMove();
		t.second.targetScanTurret();
	}
}

// Create marine add it to manager, returns marine id
unsigned int GameManager::createMarine() {
    unsigned int id = 0;
    if (!marineManager.empty()) {
        id = marineManager.rbegin()->first + 1;
    }
    marineManager[id] = Marine();
    return id;
}

bool GameManager::createMarine(SDL_Renderer* gRenderer, float x, float y){
    unsigned int id = 0;
    if (!marineManager.empty()) {
        id = marineManager.rbegin()->first + 1;
    }
    marineManager[id] = Marine();
    if (!marineManager.at(id).texture.loadFromFile("assets/texture/arrow.png", gRenderer)) {
        printf("Failed to load the marine texture!\n");
        deleteMarine(id);
        return false;
    }
    marineManager[id].setPosition(x,y);
    return true;
}

void GameManager::deleteMarine(unsigned int id) {
    marineManager.erase(id);
}


// Adds marine to level
bool GameManager::addMarine(unsigned int id, Marine& newMarine) {
    if (marineManager.count(id)) {
        return false;
    } else {
        marineManager[id] = newMarine;
        return true;
    }
}

// Get a marine by its id
Marine& GameManager::getMarine(unsigned int id) {
    return marineManager.find(id)->second;
}

// Create Turret add it to manager, returns tower id
unsigned int GameManager::createTurret() {
    unsigned int id = 0;
    if (!turretManager.empty()) {
        id = turretManager.rbegin()->first + 1;
    }
    turretManager[id] = Turret();
    return id;
}

// Deletes tower from level
void GameManager::deleteTurret(unsigned int id) {
    turretManager.erase(id);
}

// Adds tower to level
bool GameManager::addTurret (unsigned int id, Turret& newTurret) {
    if (turretManager.count(id)) {
        return false;
    } else {
        turretManager[id] = newTurret;
        return true;
    }
}

// Create turret add it to truret, returns if success
bool GameManager::createTurret(SDL_Renderer* gRenderer, float x, float y) {
    unsigned int id = 0;
    if (!turretManager.empty()) {
        id = turretManager.rbegin()->first + 1;
    }
    turretManager[id] = Turret();
    if (!turretManager.at(id).texture.loadFromFile("assets/texture/turret.png", gRenderer)) {
        printf("Failed to load the turret texture!\n");
        deleteTurret(id);
        return false;
    }
    turretManager.at(id).setPosition(x,y);
    return true;
}

// Get a tower by its id
Turret& GameManager::getTurret(unsigned int id) {
    return turretManager.find(id)->second;
}

unsigned int GameManager::addZombie(Zombie& newZombie) {
    unsigned int id = 0;
    if (!zombieManager.empty()) {
        id = zombieManager.rbegin()->first + 1;
    }
    zombieManager[id] = newZombie;
    return id;
}

// Create zombie add it to manager, returns success
bool GameManager::createZombie(SDL_Renderer* gRenderer, float x, float y) {
    unsigned int id = 0;
    if (!zombieManager.empty()) {
        id = zombieManager.rbegin()->first + 1;
    }

    zombieManager[id] = Zombie();
    if (!zombieManager.at(id).texture.loadFromFile("assets/texture/zombie.png", gRenderer)) {
        printf("Failed to load the player texture!\n");
        deleteZombie(id);
        return false;
    }

    zombieManager.at(id).setPosition(x,y);

    //Hard coded for now.  Each coordinate value must be divided by the tile size
    zombieManager.at(id).generatePath((int) x / 50, (int) y / 50, 900 / 50, 900 / 50);

    // Set end coordinates to spawn coordinates so the zombie moves on spawn
    zombieManager.at(id).setEnd(x, y);
    return true;
}

// Deletes zombie from level
void GameManager::deleteZombie(unsigned int id) {
    zombieManager.erase(id);
}

unsigned int GameManager::addObject(Object& newObject) {
    unsigned int id = 0;
    if (!objectManager.empty()) {
        id = objectManager.rbegin()->first + 1;
    }
    objectManager[id] = newObject;
    return id;
}

// Deletes Object from level
void GameManager::deleteObject(unsigned int id) {
    objectManager.erase(id);
}

unsigned int GameManager::addWeaponDrop(WeaponDrop& newWeaponDrop) {
    unsigned int id = 0;
    if (!weaponDropManager.empty()) {
        id = weaponDropManager.rbegin()->first + 1;
    }
    weaponDropManager.insert(std::make_pair(id, newWeaponDrop));
    return id;
}

// Create weapon drop add it to manager, returns success
bool GameManager::createWeaponDrop(SDL_Renderer* gRenderer, float x, float y) {
    int id;const
    int randGun = rand() % 2 + 1;

    if(randGun == 1){
        w = Rifle();
    } else if(randGun == 2){
        w = ShotGun();
    }

    if (!zombieManager.empty()) {
        id = zombieManager.rbegin()->first + 1;
    }

    weaponDropManager.insert(std::make_pair(id, WeaponDrop(w)));

    if(!weaponDropManager.at(id).texture.loadFromFile("assets/texture/shotGun.png", gRenderer)) {
        printf("Failed to load the player texture!\n");
        deleteWeaponDrop(id);
        return false;
    }
    weaponDropManager.at(id).setPosition(x,y);
    return true;
}

// Deletes weapon from level
void GameManager::deleteWeaponDrop(unsigned int id) {
    weaponDropManager.erase(id);
}

// Returns Collision Handler
CollisionHandler& GameManager::getCollisionHandler() {
    return collisionHandler;
}

// Update colliders to current state
void GameManager::updateCollider() {

    delete collisionHandler.quadtreeMov;
    delete collisionHandler.quadtreePro;
    delete collisionHandler.quadtreeDam;
    delete collisionHandler.quadtreePickUp;

    collisionHandler.quadtreeMov = new Quadtree(0, {0,0,2000,2000});
    collisionHandler.quadtreePro = new Quadtree(0, {0,0,2000,2000});
    collisionHandler.quadtreeDam = new Quadtree(0, {0,0,2000,2000});
    collisionHandler.quadtreePickUp = new Quadtree(0, {0,0,2000,2000});

    for (auto& m : marineManager) {
        collisionHandler.quadtreeMov->insert(m.second.movementHitBox.get());
        collisionHandler.quadtreePro->insert(m.second.projectileHitBox.get());
        collisionHandler.quadtreeDam->insert(m.second.damageHitBox.get());
    }

    for (auto& z : zombieManager) {
        collisionHandler.quadtreeMov->insert(z.second.movementHitBox.get());
        collisionHandler.quadtreePro->insert(z.second.projectileHitBox.get());
        collisionHandler.quadtreeDam->insert(z.second.damageHitBox.get());
    }

    for (auto& o : objectManager) {
        collisionHandler.quadtreeMov->insert(o.second.movementHitBox.get());
        collisionHandler.quadtreePro->insert(o.second.projectileHitBox.get());
        collisionHandler.quadtreeDam->insert(o.second.damageHitBox.get());
    }

    for (auto& m : turretManager) {
        collisionHandler.quadtreeMov->insert(m.second.movementHitBox.get());
        collisionHandler.quadtreePro->insert(m.second.projectileHitBox.get());
        collisionHandler.quadtreeDam->insert(m.second.damageHitBox.get());
    }

   	for (auto& b : barricadeManager) {
        if(b.second.isPlaced()) {
            collisionHandler.quadtreeMov->insert(b.second.movementHitBox.get());
            collisionHandler.quadtreeDam->insert(b.second.damageHitBox.get());
        }
	}

    for (auto& m : weaponDropManager) {
        collisionHandler.quadtreePickUp->insert(m.second.pickupHitBox.get());
    }

}

// Create barricade add it to manager, returns success
unsigned int GameManager::createBarricade(SDL_Renderer* gRenderer, float x, float y) {
    unsigned int id = 0;
    if (!barricadeManager.empty()) {
        id = barricadeManager.rbegin()->first + 1;
    }
    barricadeManager[id] = Barricade();
    if (!barricadeManager.at(id).texture.loadFromFile("assets/texture/barricade.png", gRenderer)) {
        printf("Failed to load the barricade texture!\n");
        deleteBarricade(id);
        return -1;
    }
    barricadeManager.at(id).setPosition(x,y);
    return id;
}


void GameManager::deleteBarricade(unsigned int id) {
	barricadeManager.erase(id);
}
// Get a barricade by its id
Barricade& GameManager::getBarricade(unsigned int id) {
    return barricadeManager.find(id)->second;
}

// returns the list of zombies.
// Jamie, 2017-03-01.
const std::map<unsigned int, Zombie>& GameManager::getZombies() {
    return zombieManager;
}
