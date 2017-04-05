
#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include <SDL2/SDL.h>
#include <map>
#include <unordered_map>
#include <vector>
#include <memory>
#include <iostream>
#include <cassert>

#include "../creeps/Zombie.h"
#include "../player/Marine.h"
#include "../player/Player.h"
#include "../turrets/Turret.h"
#include "../collision/CollisionHandler.h"
#include "../buildings/Object.h"
#include "../buildings/Base.h"
#include "../buildings/Wall.h"
#include "../buildings/Store.h"
#include "../buildings/Barricade.h"
#include "../UDPHeaders.h"
#include "../buildings/DropPoint.h"
#include "../map/Map.h"

#include "../inventory/WeaponDrop.h"
#include "../inventory/weapons/Weapon.h"
#include "../inventory/weapons/HandGun.h"
#include "../inventory/weapons/Rifle.h"
#include "../inventory/weapons/ShotGun.h"
#include "../inventory/WeaponDrop.h"
#include "../buildings/DropPoint.h"
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

    // Methods for creating, getting, and deleting towers from the level.
    int32_t createTurret();
    void deleteTurret(const int32_t id);

    bool addTurret(const int32_t id, const Turret& newTurret);
    int32_t createTurret(const float x, const float y) ;
    Turret& getTurret(const int32_t id);

    // Method for getting collisionHandler
    CollisionHandler& getCollisionHandler();

    void updateCollider(); // Updates CollisionHandler
    void updateMarines(const float delta); // Update marine actions
    void updateZombies(const float delta); // Update zombie actions
    void updateTurrets(); // Update turret actions

    // returns the list of zombies.
    // Jamie, 2017-03-01.
    auto& getZombies() {return zombieManager;};

    int32_t addObject(const Object&);
    void deleteObject(const int32_t id);

    int32_t addZombie(const Zombie&);
    void createZombie(const int32_t id);
    int32_t createZombie(const float x, const float y);
    void deleteZombie(const int32_t id);
    bool zombieExists(const int32_t id);
    Zombie& getZombie(const int32_t id);

    //Weapon Drops
    int32_t addWeaponDrop(WeaponDrop& newWeaponDrop);
    int32_t createWeaponDrop(const float x, const float y, const int32_t wID);
    void deleteWeaponDrop(const int32_t id);
    bool weaponDropExists(const int32_t id);
    WeaponDrop& getWeaponDrop(const int32_t id);

    //Weapons
    std::shared_ptr<Weapon> getWeapon(const int32_t id);
    void addWeapon(std::shared_ptr<Weapon> weapon);
    void removeWeapon(const int32_t id);

    int32_t createBarricade(const float x, const float y);
    void deleteBarricade(const int32_t id);
    Barricade& getBarricade(const int32_t id);

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

    int32_t createWeaponStore(const float x, const float y);//creates a weapon store
    void addStore(const int32_t id, std::shared_ptr<Store> store);//adds store to sotreManager
    bool storeExists(const int32_t id);
    std::shared_ptr<Store> getStore(const int32_t id);

    void createDropZone(const float x, const float y, const int num);
    int32_t createDropPoint(const float x, const float y);
    bool dropPointExists(const int32_t id);
    int32_t getFreeDropPointId();
    DropPoint& getDropPoint(const int32_t id);
    void freeDropPoint(const int32_t id);
    bool checkFreeDropPoints();

    // Ai Map setters and getters
    auto& getAiMap() const { return AiMap; };
    void setAiMap(const std::array<std::array<bool, M_WIDTH>, M_HEIGHT>& a) {
        AiMap = a;
    }

    //getManagers
    auto& getStoreManager() const {return storeManager;};
    auto& getTurretManager() const {return turretManager;};
    auto& getMarineManager() const {return marineManager;};
    auto& getZombieManager() const {return zombieManager;};
    auto& getWeaponDropManager() const {return weaponDropManager;};
    auto& getWeaponManager() const {return weaponManager;};
    auto& getBarricadeManager() const {return barricadeManager;};
    auto& getWallManager() const {return wallManager;};
    auto& getDropPointManager() const {return dropPointManager;};
private:
    GameManager();
    ~GameManager();
    static GameManager sInstance;
    Player player;

    CollisionHandler collisionHandler;
    std::array<std::array<bool, M_WIDTH>, M_HEIGHT> AiMap;
    std::unique_ptr<WeaponDrop> wdPointer;
    GameHashMap<int32_t, Marine> marineManager;
    GameHashMap<int32_t, Object> objectManager;
    GameHashMap<int32_t, Zombie> zombieManager;
    GameHashMap<int32_t, Turret> turretManager;
    GameHashMap<int32_t, WeaponDrop> weaponDropManager;
    GameHashMap<int32_t, std::shared_ptr<Weapon>> weaponManager;
    GameHashMap<int32_t, Barricade> barricadeManager;
    GameHashMap<int32_t, Wall> wallManager;
    GameHashMap<int32_t, std::shared_ptr<Store>> storeManager;
    GameHashMap<int32_t, DropPoint> dropPointManager;
    std::vector<int32_t> openDropPoints;
};


#endif
