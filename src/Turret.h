// Created 05/02/2017 Mark C.
#ifndef TURRET_H
#define TURRET_H
#include "LTexture.h"
#include "HitBox.h"
#include "Entity.h"
#include "Marine.h"
#include "CollisionHandler.h"
#include <vector>
#include <SDL2/SDL.h>
#include "Window.h"

const int TURRET_HEIGHT = 100;
const int TURRET_WIDTH = 100;
const int PASS_ALPHA = 200;
const int FAIL_ALPHA = 30;
const int PLACED_ALPHA = 255;
constexpr int PLACE_DISTANCE = 200;

class Turret : public Movable {
public:
    void spawnTurret(); // spawns a DEACTIVATED turret

    bool placementCheckTurret(); // checks if turret placement is within bounds

    // checks if the turret placement overlaps with any currently existing objects
    bool collisionCheckTurret(float , float , float , float , CollisionHandler &);

    void activateTurret(); // activates the turret

    void onCollision();

    void collidingProjectile(int damage);

    void damageTurret(int damage); // decrements the turrets health by damage parameter

    void decrementAmmo(int amount); // turret ammo pool decrements by this amount

    void shootTurret(); // turret shoots, this is not yet defined

    bool ammoCheckTurret();  // returns true if turret has >0 ammo, false otherwise

    bool healthCheckTurret();  // returns true if turret has >0 health, false otherwise

    bool targetScanTurret(); // checks if there are any enemies in the turret's coverage area

    void move(float, float, float, float, CollisionHandler&);

    void placeTurret();

    bool isPlaceable();

    bool isPlaced();

    void pickUpTurret(const int32_t);

    void removeTurret(); // removes the turret

    Turret(bool activated = false, int health = 200, int ammo = 100,
        bool placeable = false, bool placed = false);
    virtual ~Turret();

private:
    bool activated; // turret activated state
    int health; // turret health pool
    int ammo; // turret ammo pool
    bool placeable;
    bool placed;
};

#endif
