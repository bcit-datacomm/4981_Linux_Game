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

class Turret : public Movable {
public:
    void spawnTurret(); // spawns a DEACTIVATED turret

    bool placementCheckTurret(); // checks if turret placement is within bounds

    bool collisionCheckTurret(float x, float y, CollisionHandler& ch); // checks if the turret placement overlaps with any currently existing objects

    void activateTurret(); // activates the turret

    void onCollision();

    void collidingProjectile(int damage);

    void damageTurret(int damage); // decrements the turrets health by damage parameter

    void decrementAmmo(int amount); // turret ammo pool decrements by this amount

    void shootTurret(); // turret shoots, this is not yet defined

    bool ammoCheckTurret();  // returns true if turret has >0 ammo, false otherwise

    bool healthCheckTurret();  // returns true if turret has >0 health, false otherwise

    bool targetScanTurret(); // checks if there are any enemies in the turret's coverage area

    void removeTurret(); // removes the turret

    float getRange() const; // returns the turret's range.

    Turret();
    virtual ~Turret();

private:
    double angle = 0.0; // turret direction angle in which it is pointing
    float range = 300.0f; // turret's range.
    bool activated = false; // turret activated state
    int health = 200; // turret health pool
    int ammo = 100; // turret ammo pool
};

#endif
