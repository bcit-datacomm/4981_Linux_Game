#include <memory>
#include <utility>
#include <atomic>

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
    const int camX = cam.x;
    const int camY = cam.y;
    const int camW = cam.w;
    const int camH = cam.h;

    for (const auto& m : weaponDropManager) {
        if (m.second.getX() - camX < camW) {
            if (m.second.getY() - camY < camH) {
                Renderer::instance().render(m.second.getRelativeDestRect(cam), TEXTURES::CONCRETE);
            }
        }
    }

    for (const auto& m : marineManager) {
        if (m.second.getX() - camX < camW) {
            if (m.second.getY() - camY < camH) {
                Renderer::instance().render(m.second.getRelativeDestRect(cam), TEXTURES::MARINE,
                    m.second.getAngle());
            }
        }
    }


    for (const auto& o : objectManager) {
        if (o.second.getX() - camX < camW) {
            if (o.second.getY() - camY < camH) {
                Renderer::instance().render(o.second.getRelativeDestRect(cam), TEXTURES::CONCRETE);
            }
        }
    }

    for (const auto& z : zombieManager) {
        if (z.second.getX() - camX < camW) {
            if (z.second.getY() - camY < camH) {
                Renderer::instance().render(z.second.getRelativeDestRect(cam), TEXTURES::BABY_ZOMBIE);
            }
        }
    }

    for (const auto& m : turretManager) {
        if (m.second.getX() - camX < camW) {
            if (m.second.getY() - camY < camH) {
                Renderer::instance().render(m.second.getRelativeDestRect(cam), TEXTURES::CONCRETE,
                    m.second.getAngle());
            }
        }
    }

    for (const auto& b : barricadeManager) {
        if (b.second.getX() - camX < camW) {
            if (b.second.getY() - camY < camH) {
                Renderer::instance().render(b.second.getRelativeDestRect(cam), TEXTURES::CONCRETE);
            }
        }
    }

    for (const auto& w : wallManager) {
        if (w.second.getX() - camX < camW) {
            if (w.second.getY() - camY < camH) {
                Renderer::instance().render(w.second.getRelativeDestRect(cam), TEXTURES::CONCRETE);
            }
        }
    }
}

// Update marine movements. health, and actions
void GameManager::updateMarines(const float delta) {
    for (auto& m : marineManager) {
        m.second.move((m.second.getDX()*delta), (m.second.getDY()*delta), collisionHandler);
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
void GameManager::updateTurrets(const float delta) {
    for (auto& t : turretManager) {
        t.second.targetScanTurret();
    }
}


// Create marine add it to manager, returns marine id
int32_t GameManager::createMarine() {
    const int32_t id = generateID();
    SDL_Rect temp = {initVal, initVal, defaultSize, defaultSize};

    SDL_Rect marineRect = temp;
    SDL_Rect moveRect = temp;
    SDL_Rect projRect = temp;
    SDL_Rect damRect = temp;

    Marine m(id, marineRect, moveRect, projRect, damRect);
    marineManager.insert({id, m});
    return id;
}

bool GameManager::createMarine(const float x, const float y) {
    const int32_t id = generateID();
    SDL_Rect temp = {initVal, initVal, defaultSize, defaultSize};

    SDL_Rect marineRect = temp;
    SDL_Rect moveRect = temp;
    SDL_Rect projRect = temp;
    SDL_Rect damRect = temp;

    Marine m(id, marineRect, moveRect, projRect, damRect);
    marineManager.insert({id, m});

    marineManager.at(id).setPosition(x,y);
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

    marineManager.insert({id,newMarine});
    return true;
}

// Get a marine by its id
Marine& GameManager::getMarine(const int32_t id) {
    return marineManager.find(id)->second;
}

// Create Turret add it to manager, returns tower id
int32_t GameManager::createTurret() {
    const int32_t id = generateID();
    SDL_Rect temp = {initVal, initVal, defaultSize, defaultSize};

    SDL_Rect turretRect = temp;
    SDL_Rect moveRect = temp;
    SDL_Rect projRect = temp;
    SDL_Rect damRect = temp;
    SDL_Rect pickRect = temp;

    turretManager.insert({id, Turret(id, turretRect, moveRect, projRect, damRect, pickRect)});
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
    turretManager.insert({id, newTurret});
    return true;
}

// Create turret add it to turret, returns if success
int32_t GameManager::createTurret(const float x, const float y) {
    const int32_t id = generateID();
    SDL_Rect temp = {initVal, initVal, defaultSize, defaultSize};

    SDL_Rect turretRect = temp;
    SDL_Rect moveRect = temp;
    SDL_Rect projRect = temp;
    SDL_Rect damRect = temp;
    SDL_Rect pickRect = {initVal, initVal, PUSize, PUSize};

    turretManager.insert({id, Turret(id, turretRect, moveRect, projRect, damRect, pickRect)});
    turretManager.at(id).setPosition(x,y);
    return id;
}

// Get a tower by its id
Turret& GameManager::getTurret(const int32_t id) {
    return turretManager.find(id)->second;
}

int32_t GameManager::addZombie(const Zombie& newZombie) {
    const int32_t id = generateID();

    zombieManager.insert({id,newZombie});
    return id;
}

// Create zombie add it to manager, returns success
bool GameManager::createZombie(const float x, const float y) {
    const int32_t id = generateID();
    SDL_Rect temp = {initVal, initVal, defaultSize, defaultSize};

    SDL_Rect zombieRect = temp;
    SDL_Rect moveRect = temp;
    SDL_Rect projRect = temp;
    SDL_Rect damRect = temp;


    zombieManager.insert({id, Zombie(id, zombieRect, moveRect, projRect, damRect)});

    zombieManager.at(id).setPosition(x, y);
    zombieManager.at(id).generatePath(x, y, MAP_WIDTH / 2 - BASE_WIDTH, MAP_HEIGHT / 2 - BASE_HEIGHT);
    zombieManager.at(id).setState(ZombieState::ZOMBIE_MOVE);

    return true;
}

// Deletes zombie from level
void GameManager::deleteZombie(const int32_t id) {
    zombieManager.erase(id);

}

int32_t GameManager::addObject(const Object& newObject) {
    objectManager.insert({newObject.getId(), newObject});
    return newObject.getId();
}

// Deletes Object from level
void GameManager::deleteObject(const int32_t id) {
    objectManager.erase(id);
}


int32_t GameManager::addWeapon(std::shared_ptr<Weapon> weapon) {

    const int32_t id = weapon->getId();
    weaponManager.insert({id, weapon});

    if(weaponManager.count(id)) {
        weaponManager.at(id)->setId(id);
        return id;
    }

    return -1;

}

int32_t GameManager::addWeaponDrop(WeaponDrop& newWeaponDrop) {
    const int32_t id = newWeaponDrop.getId();

    weaponDropManager.insert({id, newWeaponDrop});
    return id;
}



// Create weapon drop add it to manager, returns success
bool GameManager::createWeaponDrop(const float x, const float y) {

    Rifle w;
    const int32_t wid = w.getId();
    const int32_t id = generateID();

    SDL_Rect weaponDropRect = {static_cast<int>(x),static_cast<int>(y),defaultSize, defaultSize};
    SDL_Rect pickRect = {static_cast<int>(x),static_cast<int>(y),defaultSize, defaultSize};

    addWeapon(std::dynamic_pointer_cast<Weapon>(std::make_shared<Rifle>(w)));

    WeaponDrop wd(id, weaponDropRect, pickRect, wid);
    weaponDropManager.insert({id, wd});

    return id;
}

//returns weapon drop in  weaponDropManager
WeaponDrop& GameManager::getWeaponDrop(const int32_t id) {
    return weaponDropManager.at(id);
}

//returns weapon in weaponManager
std::shared_ptr<Weapon> GameManager::getWeapon(const int32_t id) {

    if(weaponManager.count(id)) {
        return weaponManager.at(id);
    }

    logv("Couldnt find Weapon\n");
    return nullptr;

}

// Deletes weapon from level
void GameManager::deleteWeaponDrop(const int32_t id) {

    if(weaponDropManager.count(id)) {
        weaponDropManager.erase(id);
    } else {
        logv("Couldnt Delete Weapon Drop\n");
    }

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
    SDL_Rect temp = {initVal, initVal, defaultSize, defaultSize};

    SDL_Rect barricadeRect = temp;
    SDL_Rect moveRect = temp;
    SDL_Rect pickRect = temp;

    Barricade b(id, barricadeRect, moveRect, pickRect);
    barricadeManager.insert({id, b});

    barricadeManager.at(id).setPosition(x,y);
    return id;
}


void GameManager::deleteBarricade(const int32_t id) {
    barricadeManager.erase(id);
}
// Get a barricade by its id
Barricade& GameManager::getBarricade(const int32_t id) {
    return barricadeManager.find(id)->second;
}

// Create zombie add it to manager, returns success
int32_t GameManager::createWall(const float x, const float y, const int w, const int h) {

    const int32_t id = generateID();

    SDL_Rect wallRect = {static_cast<int>(x), static_cast<int>(y), w, h};
    SDL_Rect moveRect = {static_cast<int>(x), static_cast<int>(y), w, h};
    SDL_Rect pickRect = {static_cast<int>(x), static_cast<int>(y), w, h};

    wallManager.insert({id, Wall(id, wallRect, moveRect, pickRect, h, h)});
    return id;
}


void GameManager::setBoundary(const float startX, const float startY, const float endX, const float endY) {

    int width = endX - startX + 200;
    int height = defaultSize;

    const float x = startX - defaultSize;
    const float y = startY - defaultSize;

    createWall(x, y, width, height);
    createWall(x, endY, width, height);

    width = defaultSize;
    height = endY - startY + defaultSize;

    createWall(endX, startY, width, height);
    createWall(x, startY, width, height);

    const float sX = (endX + startX) / 2 - BASE_WIDTH - defaultSize;
    const float eX = (endX + startX) / 2 + BASE_WIDTH + defaultSize;
    const float sY = (endY + startY) / 2 - BASE_HEIGHT - defaultSize;
    const float eY = (endY + startY) / 2 + BASE_HEIGHT + defaultSize;

    width = eX - sX;
    height = defaultSize;

    createWall(sX, sY, width / 2, height);
    createWall(sX + (width / 4 * 3), sY, width / 4, height);
    createWall(sX, eY, width / 4, height);
    createWall(sX + width / 2 + defaultSize, eY, width / 2, height);

    width = defaultSize;
    height = eY - sY;

    createWall(sX, sY, width, height / 2);
    createWall(sX, sY + (height / 4 * 3), width, height / 4);
    createWall(eX, sY, width, height / 1.5);
    createWall(eX, sY + (height / 4 * 3), width, height / 4);
}

bool GameManager::createZombieWave(const int n) {

    std::vector<Point> spawnPoints;
    spawnPoints.emplace_back(Point(100, 100));
    spawnPoints.emplace_back(Point(500, 100));
    spawnPoints.emplace_back(Point(1900, 900));
    spawnPoints.emplace_back(Point(2900, 900));
    spawnPoints.emplace_back(Point(2900, 2900));
    spawnPoints.emplace_back(Point(1900, 2900));
    spawnPoints.emplace_back(Point(900, 2900));


    for (int i = 0; i < n; ++i) {
        for (const auto& p : spawnPoints) {
            createZombie(p.first, p.second);
        }
    }

    return true;

}
