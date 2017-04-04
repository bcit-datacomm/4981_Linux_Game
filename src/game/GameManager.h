
#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include <SDL2/SDL.h>
#include <map>
#include <unordered_map>
#include <vector>
#include <memory>

#include "../creeps/Zombie.h"
#include "../player/Marine.h"
#include "../turrets/Turret.h"

#include "../collision/CollisionHandler.h"

#include "../buildings/Object.h"
#include "../buildings/Base.h"
#include "../buildings/Wall.h"
#include "../buildings/Store.h"
#include "../buildings/Barricade.h"
#include "../buildings/DropPoint.h"

#include "../inventory/WeaponDrop.h"

#include "GameHashMap.h"
#include <memory>

//just for tesing weapon drop
#include "../inventory/weapons/Weapon.h"
#include "../inventory/weapons/HandGun.h"
#include "../inventory/weapons/Rifle.h"
#include "../inventory/weapons/ShotGun.h"

static constexpr int INITVAL = 0;
static constexpr int DEFAULT_SIZE = 100;
static constexpr int PUSIZE = 120;
static constexpr int DROP_POINT_SPACE = 200;//distance between drop points
static constexpr int STORE_SIZE = 400; //Store  width  and hieght length
static constexpr int STORE_PICKUP_SIZE = 50;//How much bigger the Stores PIckup hitbox is

class GameManager {
public:
    static GameManager *instance();

    int32_t generateID();

    void renderObjects(const SDL_Rect& cam); // Render all objects in level

    // Methods for creating, getting, and deleting marines from the level.
    int32_t createMarine();
    bool createMarine(const float x, const float y);
    void deleteMarine(const int32_t id);

    bool addMarine(const int32_t id, const Marine& newMarine);
    Marine& getMarine(const int32_t id);

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
    bool createZombie(const float x, const float y);
    void deleteZombie(const int32_t id);
    bool createZombieWave(const int n);
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

    CollisionHandler collisionHandler;
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
