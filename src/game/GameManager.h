#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include <SDL2/SDL.h>
#include <map>
#include <unordered_map>
#include <vector>
#include <memory>
#include <algorithm>
#include <iostream>
#include <cassert>
#include <mutex>

#include "../creeps/Zombie.h"
#include "../player/Marine.h"
#include "../player/Player.h"
#include "../collision/CollisionHandler.h"
#include "../buildings/Object.h"
#include "../buildings/Base.h"
#include "../buildings/Wall.h"
#include "../UDPHeaders.h"
#include "../map/Map.h"

#include "../inventory/weapons/Weapon.h"
#include "../inventory/weapons/HandGun.h"
#include "../inventory/weapons/Rifle.h"
#include "../inventory/weapons/ShotGun.h"
#include "GameHashMap.h"

static constexpr int INITVAL = 0;
static constexpr int DEFAULT_SIZE = 100;
static constexpr int PUSIZE = 120;
static constexpr int DROP_POINT_SPACE = 200;//distance between drop points

static constexpr int STORE_SIZE_W = 200; //Store width
static constexpr int STORE_SIZE_H = 330; //Store height
static constexpr int STORE_PICKUP_SIZE = 50;//How much bigger the Stores PIckup hitbox is
static constexpr int WEAPON_STORE_SRC_X = 183;
static constexpr int WEAPON_STORE_SRC_Y = 582;
static constexpr int WEAPON_STORE_SRC_W = 158;
static constexpr int WEAPON_STORE_SRC_H = 254;

static constexpr int TURRET_SIZE_H = 150; //Turret height
static constexpr int TURRET_PUSIZE_W = 125; // Turret pickup-hitbox width
static constexpr int TURRET_PUSIZE_H = 170; // Turret pickup-hitbox height

static constexpr int WALL_SRC_X = 15;
static constexpr int WALL_SRC_Y = 478;
static constexpr int WALL_SRC_W = 122;
static constexpr int WALL_SRC_H = 83;
static constexpr int WALL_WIDTH = 250;
static constexpr int WALL_HEIGHT = 250;

class GameManager {
public:
    static GameManager *instance();

    int32_t generateID();

    void sendServDeleteAction(const UDPHeaders type, const int32_t id) const;

    void renderObjects(const SDL_Rect& cam); // Render all objects in level

    // Methods for creating, getting, and deleting marines from the level.
    bool hasMarine(const int32_t id) const;
    int32_t createMarine();
    bool createMarine(const float x, const float y);
    void createMarine(const int32_t id);
    void deleteMarine(const int32_t id);

    const auto& getAllMarines() const {return marineManager;}
    const auto& getAllZombies() const {return zombieManager;}

    bool addMarine(const int32_t id, const Marine& newMarine);
    auto getMarine(const int32_t id) {return marineManager[id];};

    Base& getBase() {return base;}

    // Method for getting collisionHandler
    CollisionHandler& getCollisionHandler();

    void updateCollider(); // Updates CollisionHandler
    void updateMarines(const float delta); // Update marine actions
    void updateZombies(const float delta); // Update zombie actions
    void updateBase(); // Update base images

    // returns the list of zombies.
    // Jamie, 2017-03-01.
    auto& getZombies() {return zombieManager;};

    int32_t addZombie(const Zombie&);
    void createZombie(const int32_t id);
    int32_t createZombie(const float x, const float y);
    void deleteZombie(const int32_t id);
    bool zombieExists(const int32_t id);
    Zombie& getZombie(const int32_t id);

    //Weapons
    std::shared_ptr<Weapon> getWeapon(const int32_t id);
    void addWeapon(std::shared_ptr<Weapon> weapon);
    void removeWeapon(const int32_t id);

    int32_t createWall(const float x, const float y, const int h, const int w); // create Wall object

    //network update Methods
    void updateMarine(const PlayerData &playerData);
    void updateZombie(const ZombieData &zombieData);
    void handleAttackAction(const AttackAction& attackAction);

    void setPlayerUsername(int32_t id, const char * username);
    const std::string& getNameFromId(int32_t id);

    Player& getPlayer() {return player;};
    // place walls for the boundaries
    void setBoundary(const float startX, const float startY, const float endX, const float endY);

    // Ai Map setters and getters
    auto& getAiMap() const { return AiMap; };
    void setAiMap(const std::array<std::array<bool, M_WIDTH>, M_HEIGHT>& a) {
        AiMap = a;
    }

    //getManagers
    auto& getMarineManager() const {return marineManager;};
    auto& getZombieManager() const {return zombieManager;};
    auto& getWeaponManager() const {return weaponManager;};
    auto& getWallManager() {return wallManager;};

    std::mutex marineMut;
    std::mutex zombieMut;

private:
    GameManager();
    ~GameManager();
    static GameManager sInstance;
    Player player;

    Base base;

    CollisionHandler collisionHandler;
    std::array<std::array<bool, M_WIDTH>, M_HEIGHT> AiMap;
    GameHashMap<int32_t, Marine> marineManager;
    GameHashMap<int32_t, Zombie> zombieManager;
    GameHashMap<int32_t, std::shared_ptr<Weapon>> weaponManager;
    GameHashMap<int32_t, Wall> wallManager;

};


#endif
