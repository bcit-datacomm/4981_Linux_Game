#include <memory>
#include <utility>
#include <atomic>
#include <cassert>

#include "../collision/HitBox.h"
#include "../log/log.h"
#include "../game/GameManager.h"
#include "../sprites/Renderer.h"

Weapon w;
GameManager GameManager::sInstance;
//Returns the already existing GameManager or if there isn't one, makes
//a new one and returns it.
GameManager *GameManager::instance() {
    return &GameManager::sInstance;
}

int32_t GameManager::generateID() {
    static std::atomic<int32_t> counter{-1};
    return ++counter;
}

GameManager::GameManager():collisionHandler() {
    logv("Create GM\n");
}

GameManager::~GameManager() {
    logv("Destroy GM\n");
}

// Render all objects in level
void GameManager::renderObjects(const SDL_Rect& cam) {
    for (const auto& m : weaponDropManager) {
        if (m.second.getX() - cam.x < cam.w && m.second.getY() - cam.y < cam.h) {
            Renderer::instance().render(m.second.getRelativeDestRect(cam), TEXTURES::CONCRETE);
        }
    }

    for (const auto& m : marineManager) {
        if (m.second.getX() - cam.x < cam.w && m.second.getY() - cam.y < cam.h) {
            Renderer::instance().render(m.second.getRelativeDestRect(cam), TEXTURES::MARINE,
                m.second.getAngle());
        }
    }

    for (const auto& o : objectManager) {
        if (o.second.getX() - cam.x < cam.w && o.second.getY() - cam.y < cam.h) {
            Renderer::instance().render(o.second.getRelativeDestRect(cam), TEXTURES::CONCRETE);
        }
    }

    for (const auto& z : zombieManager) {
        if (z.second.getX() - cam.x < cam.w && z.second.getY() - cam.y < cam.h) {
            Renderer::instance().render(z.second.getRelativeDestRect(cam), TEXTURES::BABY_ZOMBIE);
        }
    }

    for (const auto& m : turretManager) {
        if (m.second.getX() - cam.x < cam.w && m.second.getY() - cam.y < cam.h) {
            Renderer::instance().render(m.second.getRelativeDestRect(cam), TEXTURES::CONCRETE,
                m.second.getAngle());
        }
    }

    for (const auto& b : barricadeManager) {
        if (b.second.getX() - cam.x < cam.w && b.second.getY() - cam.y < cam.h) {
            Renderer::instance().render(b.second.getRelativeDestRect(cam), TEXTURES::CONCRETE);
        }
    }

    for (const auto& w : wallManager) {
        if (w.second.getX() - cam.x < cam.w && w.second.getY() - cam.y < cam.h) {
            Renderer::instance().render(w.second.getRelativeDestRect(cam), TEXTURES::CONCRETE);
        }
    }
}

// Update marine movements. health, and actions
void GameManager::updateMarines(const float delta) {
    for (auto& m : marineManager) {
        m.second.move((m.second.getDX() * delta), (m.second.getDY() * delta), collisionHandler);
    }
}

// Update zombie movements.
void GameManager::updateZombies(const float delta) {
    for (auto& z : zombieManager) {
        z.second.generateMove();
        if (z.second.isMoving()) {
            z.second.move((z.second.getDX() * delta), (z.second.getDY() * delta), collisionHandler);
        }
    }
}

// Update turret actions.
// Jamie, 2017-03-01.
void GameManager::updateTurrets() {
    for (auto& t : turretManager) {
        t.second.targetScanTurret();
    }
}


// Create marine add it to manager, returns marine id
int32_t GameManager::createMarine() {
    const int32_t id = generateID();
    SDL_Rect temp = {INITVAL, INITVAL, DEFAULT_SIZE, DEFAULT_SIZE};

    SDL_Rect marineRect = temp;
    SDL_Rect moveRect = temp;
    SDL_Rect projRect = temp;
    SDL_Rect damRect = temp;

    marineManager.emplace(id, Marine(id, marineRect, moveRect, projRect, damRect));
    return id;
}

bool GameManager::createMarine(const float x, const float y) {
    const int32_t id = generateID();
    SDL_Rect temp = {INITVAL, INITVAL, DEFAULT_SIZE, DEFAULT_SIZE};

    SDL_Rect marineRect = temp;
    SDL_Rect moveRect = temp;
    SDL_Rect projRect = temp;
    SDL_Rect damRect = temp;

    const auto& elem = marineManager.emplace(id, Marine(id, marineRect, moveRect, projRect, damRect));
    elem->second.setPosition(x,y);
    return true;
}

void GameManager::deleteMarine(const int32_t id) {
    marineManager.erase(id);
}

// Adds marine to level
bool GameManager::addMarine(const int32_t id, const Marine& newMarine) {
    if (marineManager.count(id)) {
        return false;
    }
    marineManager.emplace(id, newMarine);
    return true;
}

// Get a marine by its id
Marine& GameManager::getMarine(const int32_t id) {
    const auto& mar = marineManager[id];
    assert(mar.second);
    return mar.first;
}

// Create Turret add it to manager, returns tower id
int32_t GameManager::createTurret() {
    const int32_t id = generateID();
    SDL_Rect temp = {INITVAL, INITVAL, DEFAULT_SIZE, DEFAULT_SIZE};

    SDL_Rect turretRect = temp;
    SDL_Rect moveRect = temp;
    SDL_Rect projRect = temp;
    SDL_Rect damRect = temp;
    SDL_Rect pickRect = temp;

    turretManager.emplace(id, Turret(id, turretRect, moveRect, projRect, damRect, pickRect));
    return id;
}

// Deletes tower from level
void GameManager::deleteTurret(const int32_t id) {
    turretManager.erase(id);
}

// Adds tower to level
bool GameManager::addTurret (const int32_t id, const Turret& newTurret) {
    if (turretManager.count(id)) {
        return false;
    }
    turretManager.emplace(id, newTurret);
    return true;
}

// Create turret add it to turret, returns if success
int32_t GameManager::createTurret(const float x, const float y) {
    const int32_t id = generateID();
    SDL_Rect temp = {INITVAL, INITVAL, DEFAULT_SIZE, DEFAULT_SIZE};

    SDL_Rect turretRect = temp;
    SDL_Rect moveRect = temp;
    SDL_Rect projRect = temp;
    SDL_Rect damRect = temp;
    SDL_Rect pickRect = {INITVAL, INITVAL, PUSIZE, PUSIZE};

    const auto& elem = turretManager.emplace(id, Turret(id, turretRect, moveRect, projRect, damRect, pickRect));
    elem->second.setPosition(x,y);
    return id;
}

// Get a tower by its id
Turret& GameManager::getTurret(const int32_t id) {
    const auto& turr = turretManager[id];
    assert(turr.second);
    return turr.first;
}

int32_t GameManager::addZombie(const Zombie& newZombie) {
    const int32_t id = generateID();
    zombieManager.emplace(id, newZombie);
    return id;
}

// Create zombie add it to manager, returns success
bool GameManager::createZombie(const float x, const float y) {
    const int32_t id = generateID();
    SDL_Rect temp = {INITVAL, INITVAL, DEFAULT_SIZE, DEFAULT_SIZE};

    SDL_Rect zombieRect = temp;
    SDL_Rect moveRect = temp;
    SDL_Rect projRect = temp;
    SDL_Rect damRect = temp;

    const auto& elem = zombieManager.emplace(id, Zombie(id, zombieRect, moveRect, projRect, damRect));
    elem->second.setPosition(x,y);
    elem->second.generatePath(x, y, MAP_WIDTH / 2 - BASE_WIDTH, MAP_HEIGHT / 2 - BASE_HEIGHT);
    elem->second.setState(ZombieState::ZOMBIE_MOVE);
    return true;
}

// Deletes zombie from level
void GameManager::deleteZombie(const int32_t id) {
    zombieManager.erase(id);
}

/*
    AUTHOR: Deric Mccadden 21/03/2017
    DESC: Checks if id can be found in zombieManager
 */
bool GameManager::zombieExists(const int32_t id) {
    return zombieManager.count(id);
}

/*
    AUTHOR: Deric Mccadden 21/03/2017
    DESC: returns zombie that matches id from zombieManager
 */
Zombie& GameManager::getZombie(const int32_t id) {
    const auto& z = zombieManager[id];
    assert(z.second);
    return z.first;
}

int32_t GameManager::addObject(const Object& newObject) {
    objectManager.emplace(newObject.getId(), newObject);
    return newObject.getId();
}

// Deletes Object from level
void GameManager::deleteObject(const int32_t id) {
    objectManager.erase(id);
}

int32_t GameManager::addWeapon(std::shared_ptr<Weapon> weapon){
    const int32_t id = weapon->getID();
    const auto& elem = weaponManager.emplace(id, weapon);
    elem->second->setId(id);
    return id;
}

int32_t GameManager::addWeaponDrop(WeaponDrop& newWeaponDrop) {
    const int32_t id = newWeaponDrop.getId();
    weaponDropManager.emplace(id, newWeaponDrop);
    return id;
}

// Create weapon drop add it to manager, returns success
bool GameManager::createWeaponDrop(const float x, const float y, int32_t wID) {
    const int32_t id = generateID();

    SDL_Rect weaponDropRect = {static_cast<int>(x),static_cast<int>(y), DEFAULT_SIZE, DEFAULT_SIZE};
    SDL_Rect pickRect = {static_cast<int>(x),static_cast<int>(y), DEFAULT_SIZE, DEFAULT_SIZE};

    weaponDropManager.emplace(id, WeaponDrop(id, weaponDropRect, pickRect, wID));
    return id;
}

/*create by maitiu March 21
 * Checks if id can be found in weaponDropManager
 */
bool GameManager::weaponDropExists(const int32_t id){
    return weaponDropManager.count(id);
}

//returns weapon drop in  weaponDropManager
WeaponDrop& GameManager::getWeaponDrop(const int32_t id) {
    logv("id: %d", id);
    const auto& wd = weaponDropManager[id];
    assert(wd.second);
    return wd.first;
}

//returns weapon in weaponManager
std::shared_ptr<Weapon> GameManager::getWeapon(const int32_t id){
    const auto& w = weaponManager[id];
    assert(w.second);
    return w.first;
}

// Deletes weapon from level
void GameManager::deleteWeaponDrop(const int32_t id) {
    weaponDropManager.erase(id);
}

// Returns Collision Handler
CollisionHandler& GameManager::getCollisionHandler() {
    return collisionHandler;
}

// Update colliders to current state
void GameManager::updateCollider() {
    collisionHandler = CollisionHandler();

    for (auto& m : marineManager) {
        collisionHandler.quadtreeMarine.insert(&m.second);
    }

    for (auto& z : zombieManager) {
        collisionHandler.quadtreeZombie.insert(&z.second);
    }

    for (auto& o : objectManager) {
        collisionHandler.quadtreeObj.insert(&o.second);
    }

    for (auto& m : turretManager) {
        if (m.second.isPlaced()) {
            collisionHandler.quadtreeTurret.insert(&m.second);
            collisionHandler.quadtreePickUp.insert(&m.second);
        }
    }

    for (auto& b : barricadeManager) {
        if (b.second.isPlaced()) {
            collisionHandler.quadtreeBarricade.insert(&b.second);
        }
    }

    for (auto& m : weaponDropManager) {
        collisionHandler.quadtreePickUp.insert(&m.second);
    }

    for (auto& w : wallManager) {
        collisionHandler.quadtreeWall.insert(&w.second);
    }
}

// Create barricade add it to manager, returns success
int32_t GameManager::createBarricade(const float x, const float y) {
    const int32_t id = generateID();
    SDL_Rect temp = {INITVAL, INITVAL, DEFAULT_SIZE, DEFAULT_SIZE};

    SDL_Rect barricadeRect = temp;
    SDL_Rect moveRect = temp;
    SDL_Rect pickRect = temp;

    const auto& elem = barricadeManager.emplace(id, Barricade(id, barricadeRect, moveRect, pickRect));
    elem->second.setPosition(x,y);
    return id;
}

void GameManager::deleteBarricade(const int32_t id) {
    barricadeManager.erase(id);
}

// Get a barricade by its id
Barricade& GameManager::getBarricade(const int32_t id) {
    const auto& bar = barricadeManager[id];
    assert(bar.second);
    return bar.first;
}

// Create zombie add it to manager, returns success
int32_t GameManager::createWall(const float x, const float y, const int w, const int h) {

    const int32_t id = generateID();

    SDL_Rect wallRect = {static_cast<int>(x), static_cast<int>(y), w, h};
    SDL_Rect moveRect = {static_cast<int>(x), static_cast<int>(y), w, h};
    SDL_Rect pickRect = {static_cast<int>(x), static_cast<int>(y), w, h};

    wallManager.emplace(id, Wall(id, wallRect, moveRect, pickRect, h, h));
    return id;
}

void GameManager::setBoundary(const float startX, const float startY, const float endX, const float endY) {

    int width = endX - startX + 200;
    int height = DEFAULT_SIZE;

    const float x = startX - DEFAULT_SIZE;
    const float y = startY - DEFAULT_SIZE;

    createWall(x, y, width, height);
    createWall(x, endY, width, height);

    width = DEFAULT_SIZE;
    height = endY - startY + DEFAULT_SIZE;

    createWall(endX, startY, width, height);
    createWall(x, startY, width, height);

    const float sX = (endX + startX) / 2 - BASE_WIDTH - DEFAULT_SIZE;
    const float eX = (endX + startX) / 2 + BASE_WIDTH + DEFAULT_SIZE;
    const float sY = (endY + startY) / 2 - BASE_HEIGHT - DEFAULT_SIZE;
    const float eY = (endY + startY) / 2 + BASE_HEIGHT + DEFAULT_SIZE;

    width = eX - sX;
    height = DEFAULT_SIZE;

    createWall(sX, sY, width / 2, height);
    createWall(sX + (width / 4 * 3), sY, width / 4, height);
    createWall(sX, eY, width / 4, height);
    createWall(sX + width / 2 + DEFAULT_SIZE, eY, width / 2, height);

    width = DEFAULT_SIZE;
    height = eY - sY;

    createWall(sX, sY, width, height / 2);
    createWall(sX, sY + (height / 4 * 3), width, height / 4);
    createWall(eX, sY, width, height / 1.5);
    createWall(eX, sY + (height / 4 * 3), width, height / 4);
}

bool GameManager::createZombieWave(const int n) {
    std::vector<Point> spawnPoints;
    spawnPoints.emplace_back(100, 100);
    spawnPoints.emplace_back(500, 100);
    spawnPoints.emplace_back(1900, 900);
    spawnPoints.emplace_back(2900, 900);
    spawnPoints.emplace_back(2900, 2900);
    spawnPoints.emplace_back(1900, 2900);
    spawnPoints.emplace_back(900, 2900);

    for (int i = 0; i < n; ++i) {
        for (const auto& p : spawnPoints) {
            createZombie(p.first, p.second);
        }
    }
    return true;
}
