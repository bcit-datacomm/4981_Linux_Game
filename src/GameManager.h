#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H
#include<SDL2/SDL.h>
#include "Marine.h"
#include "Turret.h"
#include "CollisionHandler.h"
#include "Object.h"
#include "Zombie.h"
#include "Barricade.h"
#include <map>
#include <unordered_map>
#include <vector>
#include "UDPHeaders.h"
#include <memory>
#include "WeaponDrop.h"
#include <iostream>
#include <memory>

//just for tesing weapon drop
#include "Weapon.h"
#include "HandGun.h"
#include "Rifle.h"
#include "ShotGun.h"

class GameManager {
public:
    static GameManager *instance();
    ~GameManager();

    void renderObjects(SDL_Renderer* gRenderer, float camX, float camY); // Render all objects in level


    // Methods for creating, getting, and deleting marines from the level.
    unsigned int createMarine();
    Marine& createMarine(int id, SDL_Renderer* gRenderer, float x, float y);
    void deleteMarine(unsigned int id);

    bool addMarine(unsigned int id, Marine& newMarine);
    Marine& getMarine(unsigned int id);
    const auto& getAllMarines() const {return marineManager;}
	const auto& getAllZombies() const {return zombieManager;}

    // Methods for creating, getting, and deleting towers from the level.
    unsigned int createTurret();
    void deleteTurret(unsigned int id);

    bool addTurret(unsigned int id, Turret& newTurret);
    bool createTurret(SDL_Renderer* gRenderer, float x, float y) ;
    Turret& getTurret(unsigned int id);

    // Method for getting collisionHandler
    CollisionHandler& getCollisionHandler();

    void updateCollider(); // Updates CollisionHandler
    void updateMarines(const float& delta); // Update marine actions
    void updateZombies(const float& delta); // Update zombie actions

    unsigned int addObject(Object&);
    void deleteObject(unsigned int id);

    unsigned int addZombie(Zombie&);
    bool createZombie(SDL_Renderer* gRenderer, float x, float y);
    void deleteZombie(unsigned int id);

    unsigned int addWeaponDrop(WeaponDrop& newWeaponDrop);
    bool createWeaponDrop(SDL_Renderer* gRenderer, float x, float y);
    void deleteWeaponDrop(unsigned int id);

    //network update Methods
    void updateMarine(const PlayerData &playerData);
    void updateZombie(const ZombieData &zombieData);
    void printMarineCount() {std::cout << "marine count: " << marineManager.size() << std::endl;};
    //temporary dont yell at me
    void setRenderer(SDL_Renderer *renderer) {_renderer = renderer;};

    unsigned int createBarricade(SDL_Renderer* gRenderer, float x, float y);
    void deleteBarricade(unsigned int id);
    Barricade& getBarricade(unsigned int id);
private:
    static GameManager *sInstance;

    //used a ptr because it is a temporary solution
    SDL_Renderer *_renderer;

    GameManager();
    CollisionHandler collisionHandler;
    std::unique_ptr<WeaponDrop> wdPointer;
    std::map<unsigned int, Marine> marineManager;
    std::map<unsigned int, Object> objectManager;
    std::map<unsigned int, Zombie> zombieManager;
    std::map<unsigned int, Turret> turretManager;
    std::map<unsigned int, WeaponDrop> weaponDropManager;
    std::map<unsigned int, Barricade> barricadeManager;

};


#endif
