#include <omp.h>
#include <memory>
#include <utility>
#include <atomic>
#include <cassert>

#include "../collision/HitBox.h"
#include "../log/log.h"
#include "../game/GameManager.h"
#include "../sprites/Renderer.h"
#include "../server/servergamestate.h"

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

/**
 * Date: Feb. 4, 2017
 * Modified: ----
 * Author: Jacob McPhail
 * Function Interface: GameManager()
 * Description:
 *     ctor for the game manager.
 */
GameManager::GameManager() : collisionHandler() {
    logv("Create GM\n");
}

/**
 * Date: Feb. 4, 2017
 * Modified: ----
 * Author: Jacob McPhail
 * Function Interface: ~GameManager()
 * Description:
 *     dctor for the game manager.
 */
GameManager::~GameManager() {
    logv("Destroy GM\n");
}

/**
 * Date: Feb. 4, 2017
 *
 * Author: Jacob McPhail
 *
 * Modified: Mar. 15, 2017 - Mark Tattrie
 * Modified: Apr. 02, 2017 - Terry Kang
 *  Set alpha to the sprite of Brricade if it is not placeable
 * Modified: Apr. 07, 2017 - Isaac Morneau
 *      cleaned up the inersection calls, removed object rendering entirely
 *
 * Function Interface: void GameManager::renderObjects(const SDL_Rect& cam)
 *
 * Description:
 *     Render all objects in level
 */
void GameManager::renderObjects(const SDL_Rect& cam) {
    for (const auto& o : marineManager) {
        if (SDL_HasIntersection(&cam, &o.second.getDestRect())) {
            const auto& dest = o.second.getRelativeDestRect(cam);
            const auto angle = o.second.getAngle() - 90;

            if (-180 < angle && 0 > angle) {
                Weapon* weapon = o.second.inventory.getCurrent();
                if (weapon) {
                    weapon->updateGunRender(o.second, cam);
                }
                Renderer::instance().render(dest,
                    o.second.getId() % 2 ? TEXTURES::MARINE : TEXTURES::COWBOY,
                    o.second.getSrcRect());
            } else {
                Renderer::instance().render(dest,
                    o.second.getId() % 2 ? TEXTURES::MARINE : TEXTURES::COWBOY,
                    o.second.getSrcRect());
                Weapon* weapon = o.second.inventory.getCurrent();
                if (weapon) {
                    weapon->updateGunRender(o.second, cam);
                }
            }
        }
    }

    if (SDL_HasIntersection(&cam, &base.getDestRect())) {
        Renderer::instance().render(base.getRelativeDestRect(cam), TEXTURES::BASE,
            base.getSrcRect());
    }

    for (const auto& o : zombieManager) {
        if (SDL_HasIntersection(&cam, &o.second.getDestRect())) {
            Renderer::instance().render(o.second.getRelativeDestRect(cam),
                    o.second.getId() % 2 ? TEXTURES::BABY_ZOMBIE : TEXTURES::DIGGER_ZOMBIE,
                    o.second.getSrcRect());
        }
    }

    for (const auto& o : wallManager) {
        if (SDL_HasIntersection(&cam, &o.second.getDestRect())) {
            static constexpr SDL_Rect WALL_SRC_RECT = {WALL_SRC_X, WALL_SRC_Y, WALL_SRC_W, WALL_SRC_H};
            Renderer::instance().render(o.second.getRelativeDestRect(cam), TEXTURES::MAP_OBJECTS,
                WALL_SRC_RECT, WALL_WIDTH, WALL_HEIGHT);
        }
    }
}

/**
 * Date: Feb. 4, 2017
 * Modified: ----
 * Author: Jacob McPhail
 * Function Interface: (const float delta)
 *      delta : Delta time to control frame rate.
 *
 * Description:
 *     Update marine movements. health, and actions
 */
void GameManager::updateMarines(const float delta) {
#pragma omp parallel
#pragma omp single
    {
        for (auto it = marineManager.begin(); it != marineManager.end(); ++it) {
#pragma omp task firstprivate(it)
            {
                if (!networked) {
                    it->second.move((it->second.getDX() * delta), (it->second.getDY() * delta), collisionHandler);
                }
#ifndef SERVER
                it->second.updateImageDirection();
                it->second.updateImageWalk();
#endif
            }
        }
#pragma omp taskwait
    }
}

// Update zombie movements.
void GameManager::updateZombies(const float delta) {
#pragma omp parallel
#pragma omp single
    {
        for (auto it = zombieManager.begin(); it != zombieManager.end(); ++it) {
#pragma omp task firstprivate(it)
            {
                it->second.update();
                it->second.move((it->second.getDX() * delta), (it->second.getDY() * delta), collisionHandler);
#ifndef SERVER
                it->second.updateImageDirection();
                it->second.updateImageWalk();
#endif
            }
        }
#pragma omp taskwait
    }
}

/**
* Date: April 6, 2017
* Designer: Trista Huang
* Programmer: Trista Huang
* Function Interface: void GameManager::updateBase()
* Description:
*       This function calls function to check for base health everytime an update happens,
*       and changes base image accordingly.
*       It is called from GameStateMatch every update.
*/
void GameManager::updateBase() {
    base.updateBaseImage();
}

bool GameManager::hasMarine(const int32_t id) const {
    return marineManager.count(id);
}

/**
 * Date: Feb. 4, 2017
 * Modified: ----
 * Author: Jacob McPhail
 * Function Interface: createMarine()
 * Description:
 *     Create marine add it to manager, returns marine id
 */
int32_t GameManager::createMarine() {
    const int32_t id = generateID();
    SDL_Rect temp = {INITVAL, INITVAL, MARINE_WIDTH, MARINE_HEIGHT};

    SDL_Rect marineRect = temp;
    SDL_Rect moveRect = temp;
    SDL_Rect projRect = temp;
    SDL_Rect damRect = temp;

    marineManager.emplace(id, Marine(id, marineRect, moveRect, projRect, damRect));
    return id;
}

/**
 * Date: Mar. 1, 2017
 * Modified: Mar. 15 2017 - Mark Tattrie
 * Author: Jacob McPhail
 * Function Interface: bool GameManager::createMarine(const float x, const float y) {
 * Description:
 *     Create a marine at position x,y and add it to the marine manager
 */
bool GameManager::createMarine(const float x, const float y) {
    const int32_t id = generateID();
    SDL_Rect temp = {INITVAL, INITVAL, MARINE_WIDTH, MARINE_HEIGHT};

    SDL_Rect marineRect = temp;
    SDL_Rect moveRect = temp;
    SDL_Rect projRect = temp;
    SDL_Rect damRect = temp;

    const auto& elem = marineManager.emplace(id, Marine(id, marineRect, moveRect, projRect, damRect));
    elem->second.setPosition(x,y);
    return true;
}

void GameManager::createMarine(const int32_t id) {
    SDL_Rect temp = {INITVAL, INITVAL, MARINE_WIDTH, MARINE_HEIGHT};

    SDL_Rect marineRect = temp;
    SDL_Rect moveRect = temp;
    SDL_Rect projRect = temp;
    SDL_Rect damRect = temp;

    marineManager.emplace(id, Marine(id, marineRect, moveRect, projRect, damRect));
}

/**
 * Date: Mar. 1, 2017
 * Modified: Mar. 15 2017 - Mark Tattrie
 * Author:
 * Function Interface: void GameManager::deleteMarine(const int32_t id) {
 * Description:
 * remove the marine by its id from the marineManager
 */
void GameManager::deleteMarine(const int32_t id) {
    marineManager.erase(id);
#ifdef SERVER
    saveDeletion({UDPHeaders::MARINE, id});
#endif
}

/**
 * Date: Feb. 4, 2017
 * Modified: ----
 * Author: Jacob McPhail
 * Function Interface: addMarine(const int32_t id, const Marine& newMarine)
 *      id : Marine id
 *      newMarine : Marine to add the manager
 *
 * Description:
 *     Adds marine to level.
 */
bool GameManager::addMarine(const int32_t id, const Marine& newMarine) {
    if (marineManager.count(id)) {
        return false;
    }
    marineManager.emplace(id, newMarine);
    return true;
}

/**
 * Date: Feb. 4, 2017
 * Modified: ----
 * Author: Jacob McPhail
 * Function Interface: getMarine(const int32_t id)
 *      id : Marine id
 *
 * Description:
 *     Get a marine by its id
 */
/*
Marine& GameManager::getMarine(const int32_t id) {
    const auto& mar = marineManager[id];
    assert(mar.second);
    return mar.first;
}*/

/**
 * Date: Feb. 8, 2017
 * Modified: ----
 * Author: Jacob McPhail
 * Function Interface: addZombie(const Zombie& newZombie)
 *      newZombie : Zombie to add
 *
 * Description:
 *     Add a zombie to the manager.
 */
int32_t GameManager::addZombie(const Zombie& newZombie) {
    const int32_t id = generateID();
    zombieManager.emplace(id, newZombie);
    return id;
}

void GameManager::createZombie(const int32_t id) {
    SDL_Rect temp = {INITVAL, INITVAL, DEFAULT_SIZE, DEFAULT_SIZE};

    SDL_Rect zombieRect = temp;
    SDL_Rect moveRect = temp;
    SDL_Rect projRect = temp;
    SDL_Rect damRect = temp;

    zombieManager.emplace(id, Zombie(id, zombieRect, moveRect, projRect, damRect));
}

/**
* Date: Mar. 1, 2017
* Modified: Mar. 15 2017 - Mark Tattrie
* Author: Jacob McPhail
* Function Interface: bool GameManager::createZombie(const float x, const float y)
* Description:
*   Create zombie add it to manager, returns success
*/
int32_t GameManager::createZombie(const float x, const float y) {
    const int32_t id = generateID();
    SDL_Rect temp = {INITVAL, INITVAL, ZOMBIE_WIDTH, ZOMBIE_HEIGHT};

    SDL_Rect zombieRect = temp;
    SDL_Rect moveRect = temp;
    SDL_Rect projRect = temp;
    SDL_Rect damRect = temp;

    const auto& elem = zombieManager.emplace(id, Zombie(id, zombieRect, moveRect, projRect, damRect));
    elem->second.setPosition(x,y);
    return id;
}

/**
 * Date: Feb. 8, 2017
 * Modified: ----
 * Author: Jacob McPhail
 * Function Interface: deleteZombie(const int32_t id)
 *      id : Zombie id
 *
 * Description:
 *     Deletes zombie from level.
 */
void GameManager::deleteZombie(const int32_t id) {
    zombieManager.erase(id);
#ifdef SERVER
    saveDeletion({UDPHeaders::ZOMBIE, id});
#endif
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
//Created By Maitiu
//Adds Weapon to Weapon Manager
void GameManager::addWeapon(std::shared_ptr<Weapon> weapon) {
    weaponManager.emplace(weapon->getID(), weapon);
}

/*
 *Created By Maitiu March 30 2017
 */
void GameManager::removeWeapon(const int32_t id) {
    weaponManager.erase(id);
}

//created by Maitiu 2017-03-12
//returns weapon in weaponManager using id
std::shared_ptr<Weapon> GameManager::getWeapon(const int32_t id) {
    const auto& w = weaponManager[id];
    assert(w.second);
    return w.first;
}

// Returns Collision Handler
CollisionHandler& GameManager::getCollisionHandler() {
    return collisionHandler;
}

/**
 * Date: Feb. 4, 2017
 * Modified: Mar. 15, 2017 - Mark Tattrie
 * Modified: Apr. 5, 2017 - John Agapeyev
 *      added openMP
 * Modified: Apr. 7, 2017 - Isaac Morneau
 *      removed object manager, added base to walls
 * Author: Jacob McPhail
 * Function Interface: void GameManager::updateCollider()
 * Description:
 *     Update colliders to current state
 */
void GameManager::updateCollider() {
    collisionHandler.clear();

    //adding the base to the wall manager
    //this way we dont need the object manager at all
    collisionHandler.insertWall(&base);

#pragma omp parallel sections shared(collisionHandler)
    {
#pragma omp section
        for (auto& m : marineManager) {
            collisionHandler.insertMarine(&m.second);
        }

#pragma omp section
        for (auto& z : zombieManager) {
            collisionHandler.insertZombie(&z.second);
        }

#pragma omp section
        for (auto& w : wallManager) {
            collisionHandler.insertWall(&w.second);
        }
    }
}

/**
Date: 30. 17, 2017
Programmer: Brody McCrone
Interface: void GameManager::updateMarine(const PlayerData &playerData)
    playerData: Player data struct received from the server containing
        updated player info.
Description:
Checks if there is a marine in the marineManager with the id in the
playData struct, if not it creates a marine with that id. Whether it
created it or not it updates it's positition angle and health.
*/
void GameManager::updateMarine(const PlayerData &playerData) {
    if (marineManager.count(playerData.playerid) == 0) {
        createMarine(playerData.playerid);
    }
    Marine& marine = marineManager[playerData.playerid].first;
    marine.setPosition(playerData.xpos, playerData.ypos);
    marine.setDX(playerData.xdel);
    marine.setDY(playerData.ydel);
    marine.setAngle(playerData.direction);
    marine.setHealth(playerData.health);
}

/**
Date: 30. 17, 2017
Programmer: Brody McCrone
Interface: void GameManager::updateZombie(const ZombieData &zombieData)
    zobmieData: Zombie data struct received from the server containing
        updated zombie info.
Description:
Checks if there is a zombie in the zombieManager with the id in the
playData struct, if not it creates that zombie with that id. Whether
it created it or not it updates it's positition angle and health.
*/
void GameManager::updateZombie(const ZombieData &zombieData) {
    if(zombieManager.find(zombieData.zombieid) == zombieManager.end()) {
        createZombie(zombieData.zombieid);
    }
    Zombie& zombie = zombieManager[zombieData.zombieid].first;
    zombie.setPosition(zombieData.xpos, zombieData.ypos);
    zombie.setAngle(zombieData.direction);
    zombie.setHealth(zombieData.health);
}

/**
Date: 30. 17, 2017
Programmer: Brody McCrone and Deric Mccadden
Interface: void GameManager::handleAttackAction(const AttackAction& attackAction)
    attackAction: Information about an attack a marine performed received from
        the server.
Description:
-Doesn't update the players marine, because the player performs actions before
sending information to them about the server.
-If the marine exits, it fires its current weapon. Weapon id is in the attack
action but support for weapon ids hasn't been implemented so ignores it and
fires current weapon.
*/
void GameManager::handleAttackAction(const AttackAction& attackAction) {
    if (!(attackAction.playerid == player.getId())) {
        auto marine = marineManager[attackAction.playerid];
        if (marine.second) {
            int curX = marine.first.getX();
            int curY = marine.first.getY();
            double curAngle = marine.first.getAngle();
            marine.first.setPosition(attackAction.xpos, attackAction.ypos);
            marine.first.setAngle(attackAction.direction);
            marine.first.fireWeapon();
            marine.first.setPosition(curX, curY);
            marine.first.setAngle(curAngle);
        }
    }
}


/**
* Date: Mar. 14, 2017
* Modified: Mar. 15 2017 - Mark Tattrie
*           Mar. 16 2017 - Michael Goll
* Author: Maitiu Morton
* Function Interface: int32_t GameManager::createWall(const float x, const float y, const int w,
*       const int h)
* Description:
* Create wall, add it to manager, returns success
*/
int32_t GameManager::createWall(const float x, const float y, const int w, const int h) {
    const int32_t id = generateID();

    SDL_Rect wallRect = {static_cast<int>(x), static_cast<int>(y), w, h};
    SDL_Rect moveRect = {static_cast<int>(x), static_cast<int>(y), w, h};
    SDL_Rect pickRect = {static_cast<int>(x), static_cast<int>(y), w, h};

    wallManager.emplace(id, Wall(id, wallRect, moveRect, pickRect, h, h));
    return id;
}

/**
* Date: Mar. 1, 2017
* Modified: Mar. 8 2017 - Jacob McPhail
* Author: Terry Kang
* Function Interface: setBoundary(const float startX, const float startY,
*        const float endX, const float endY)
* Description:
*   Create test area.
*/
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
