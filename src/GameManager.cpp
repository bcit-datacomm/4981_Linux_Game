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

GameManager::GameManager() {
    printf("Create GM\n");
    this->collisionHandler = new CollisionHandler();
}

GameManager::~GameManager() {
    printf("Destroy GM\n");
    delete collisionHandler;
    marineManager.clear();
    zombieManager.clear();
    objectManager.clear();
    turretManager.clear();
    weaponDropManager.clear();
}

// Render all objects in level
void GameManager::renderObjects(SDL_Renderer* gRenderer, float camX, float camY) {

    for (const auto& m : this->marineManager) {
        m.second.texture.render(gRenderer, m.second.getX()-camX, m.second.getY()-camY,
                                 NULL, m.second.getAngle());
    }
    /*for (const auto& o : this->objectManager) {
        o.second.texture.render(gRenderer, o.second.getX()-camX, o.second.getY()-camY);
    }*/
    for (const auto& z : this->zombieManager) {
        z.second.texture.render(gRenderer, z.second.getX()-camX, z.second.getY()-camY,
                                 NULL, z.second.getAngle());
    }

    for (const auto& m : this->turretManager) {
        m.second.texture.render(gRenderer, m.second.getX()-camX, m.second.getY()-camY,
                                 NULL, m.second.getAngle());
    }

    for (const auto& m : this->weaponDropManager) {
        m.second.texture.render(gRenderer, m.second.getX()-camX, m.second.getY()-camY);
    }
}

// Update marine movements. health, and actions
void GameManager::updateMarines(const float& delta) {
    for (auto& m : this->marineManager) {
        m.second.move((m.second.getDX()*delta), (m.second.getDY()*delta), this->collisionHandler);
    }
}

// Update zombie movements.
void GameManager::updateZombies(const float& delta) {
    for (auto& z : this->zombieManager) {
        z.second.generateRandomMove();
        z.second.move((z.second.getDX()*delta), (z.second.getDY()*delta), this->collisionHandler);
    }
}
// Create marine add it to manager, returns marine id
unsigned int GameManager::createMarine() {
    unsigned int id = 0;
    if (!this->marineManager.empty()) {
        id = this->marineManager.rbegin()->first + 1;
    }
    this->marineManager[id] = Marine();
    return id;
}

bool GameManager::createMarine(SDL_Renderer* gRenderer, float x, float y){
    unsigned int id = 0;
    if (!this->marineManager.empty()) {
        id = this->marineManager.rbegin()->first + 1;
    }
    this->marineManager[id] = Marine();
    if (!this->marineManager.at(id).texture.loadFromFile("assets/texture/turret.png", gRenderer)) {
        printf("Failed to load the turret texture!\n");
        this->deleteTurret(id);
        return false;
    }
    this->marineManager[id].setPosition(x,y);
    return true;
}

void GameManager::deleteMarine(unsigned int id) {
    this->marineManager.erase(id);
}


// Adds marine to level
bool GameManager::addMarine(unsigned int id, Marine& newMarine) {
    if (this->marineManager.count(id)) {
        return false;
    } else {
        this->marineManager[id] = newMarine;
        return true;
    }
}

// Get a marine by its id
Marine* GameManager::getMarine(unsigned int id) {
    return &(this->marineManager.find(id)->second);
}

// Create Turret add it to manager, returns tower id
unsigned int GameManager::createTurret() {
    unsigned int id = 0;
    if (!this->turretManager.empty()) {
        id = this->turretManager.rbegin()->first + 1;
    }
    this->turretManager[id] = Turret();
    return id;
}

// Deletes tower from level
void GameManager::deleteTurret(unsigned int id) {
    this->turretManager.erase(id);
}

// Adds tower to level
bool GameManager::addTurret (unsigned int id, Turret& newTurret) {
    if (this->turretManager.count(id)) {
        return false;
    } else {
        this->turretManager[id] = newTurret;
        return true;
    }
}

// Create turret add it to truret, returns if success
bool GameManager::createTurret(SDL_Renderer* gRenderer, float x, float y) {
    unsigned int id = 0;
    if (!this->turretManager.empty()) {
        id = this->turretManager.rbegin()->first + 1;
    }
    this->turretManager[id] = Turret();
    if (!this->turretManager.at(id).texture.loadFromFile("assets/texture/turret.png", gRenderer)) {
        printf("Failed to load the turret texture!\n");
        this->deleteTurret(id);
        return false;
    }
    this->turretManager.at(id).setPosition(x,y);
    return true;
}

// Get a tower by its id
Turret* GameManager::getTurret(unsigned int id) {
    return &(this->turretManager.find(id)->second);
}

unsigned int GameManager::addZombie(Zombie& newZombie) {
    unsigned int id = 0;
    if (!this->zombieManager.empty()) {
        id = this->zombieManager.rbegin()->first + 1;
    }
    zombieManager[id] = newZombie;
    return id;
}

// Create zombie add it to manager, returns success
bool GameManager::createZombie(SDL_Renderer* gRenderer, float x, float y) {
    unsigned int id = 0;
    if (!this->zombieManager.empty()) {
        id = this->zombieManager.rbegin()->first + 1;
    }
    this->zombieManager[id] = Zombie();
    if (!this->zombieManager.at(id).texture.loadFromFile("assets/texture/zombie.png", gRenderer)) {
        printf("Failed to load the player texture!\n");
        this->deleteZombie(id);
        return false;
    }
    this->zombieManager.at(id).setPosition(x,y);
    return true;
}

// Deletes zombie from level
void GameManager::deleteZombie(unsigned int id) {
    this->zombieManager.erase(id);
}

unsigned int GameManager::addObject(Object& newObject) {
    unsigned int id = 0;
    if (!this->objectManager.empty()) {
        id = this->objectManager.rbegin()->first + 1;
    }
    objectManager[id] = newObject;
    return id;
}

// Deletes Object from level
void GameManager::deleteObject(unsigned int id) {
    this->objectManager.erase(id);
}

unsigned int GameManager::addWeaponDrop(WeaponDrop& newWeaponDrop) {
    unsigned int id = 0;
    if (!this->weaponDropManager.empty()) {
        id = this->weaponDropManager.rbegin()->first + 1;
    }
    weaponDropManager.insert(std::make_pair(id, newWeaponDrop));
    return id;
}

// Create weapon drop add it to manager, returns success
bool GameManager::createWeaponDrop(SDL_Renderer* gRenderer, float x, float y) {
    int id;
    int randGun = rand() % 2 + 1;

    if(randGun == 1){
        w = Rifle();
    } else if(randGun == 2){
        w = ShotGun();
    }

    if (!this->zombieManager.empty()) {
        id = this->zombieManager.rbegin()->first + 1;
    }

    weaponDropManager.insert(std::make_pair(id, WeaponDrop(w)));

    if(!this->weaponDropManager.at(id).texture.loadFromFile("assets/texture/shotGun.png", gRenderer)) {
        printf("Failed to load the player texture!\n");
        this->deleteWeaponDrop(id);
        return false;
    }
    this->weaponDropManager.at(id).setPosition(x,y);
    return true;
}

// Deletes weapon from level
void GameManager::deleteWeaponDrop(unsigned int id) {
    this->weaponDropManager.erase(id);
}

// Returns Collision Handler
CollisionHandler* GameManager::getCollisionHandler() {
    return this->collisionHandler;
}

// Update colliders to current state
void GameManager::updateCollider() {

    delete this->collisionHandler->quadtreeMov;
    delete this->collisionHandler->quadtreePro;
    delete this->collisionHandler->quadtreeDam;

    this->collisionHandler->quadtreeMov = new Quadtree(0, {0,0,2000,2000});
    this->collisionHandler->quadtreePro = new Quadtree(0, {0,0,2000,2000});
    this->collisionHandler->quadtreeDam = new Quadtree(0, {0,0,2000,2000});


    for (auto& m : this->marineManager) {
        this->collisionHandler->quadtreeMov->insert(&m.second.movementHitBox);
        this->collisionHandler->quadtreePro->insert(&m.second.projectileHitBox);
        this->collisionHandler->quadtreeDam->insert(&m.second.damageHitBox);
        this->collisionHandler->quadtreePickUp->insert(&m.second.pickUpHitBox);
    }
    std::vector<HitBox> projectileColliders;
    for (const auto& m : this->marineManager) {
        projectileColliders.push_back(m.second.projectileHitBox);
    }
    for (auto& z : this->zombieManager) {
        this->collisionHandler->quadtreeMov->insert(&z.second.movementHitBox);
        this->collisionHandler->quadtreePro->insert(&z.second.projectileHitBox);
        this->collisionHandler->quadtreeDam->insert(&z.second.damageHitBox);
    }

    for (auto& o : this->objectManager) {
        this->collisionHandler->quadtreeMov->insert(&o.second.movementHitBox);
        this->collisionHandler->quadtreePro->insert(&o.second.projectileHitBox);
        this->collisionHandler->quadtreeDam->insert(&o.second.damageHitBox);
    }

      for (auto& m : this->turretManager) {
        this->collisionHandler->quadtreeMov->insert(&m.second.movementHitBox);
        this->collisionHandler->quadtreePro->insert(&m.second.projectileHitBox);
        this->collisionHandler->quadtreeDam->insert(&m.second.damageHitBox);
    }

    for (auto& m : this->weaponDropManager) {
        this->collisionHandler->quadtreePickUp->insert(&m.second.pickUpHitBox);
    }

}
