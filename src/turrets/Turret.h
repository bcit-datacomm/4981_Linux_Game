// Created 05/02/2017 Mark C.
#ifndef TURRET_H
#define TURRET_H
#include "../sprites/LTexture.h"
#include "../collision/HitBox.h"
#include "../basic/Entity.h"
#include "../player/Marine.h"
#include "../collision/CollisionHandler.h"
#include <vector>
#include <SDL2/SDL.h>
#include "../view/Window.h"

constexpr int TURRET_HEIGHT = 100;
constexpr int TURRET_WIDTH = 100;
constexpr int PASS_ALPHA = 200;
constexpr int FAIL_ALPHA = 30;
constexpr int PLACED_ALPHA = 255;
constexpr int PLACE_DISTANCE = 200;

class Turret : public Movable {
public:
    Turret(bool activated = false, int health = 200, int ammo = 100,
            bool boolPlaced = false, float range = 400.0f);
    virtual ~Turret();

    void spawnTurret(); // spawns a DEACTIVATED turret

    bool placementCheckTurret(); // checks if turret placement is within bounds

    // checks if the turret placement overlaps with any currently existing objects
    bool collisionCheckTurret(const float , const float , const float , const float , CollisionHandler &);

    void activateTurret(); // activates the turret

    void onCollision();

    void collidingProjectile(const int damage);

    void damageTurret(const int damage); // decrements the turrets health by damage parameter

    void decrementAmmo(const int amount); // turret ammo pool decrements by this amount

    void shootTurret(); // turret shoots, this is not yet defined

    bool ammoCheckTurret();  // returns true if turret has >0 ammo, false otherwise

    bool healthCheckTurret();  // returns true if turret has >0 health, false otherwise

    bool targetScanTurret(); // checks if there are any enemies in the turret's coverage area

    void move(float, float, float, float, CollisionHandler&);

    void placeTurret();

    bool isPlaced();

    void pickUpTurret();

    void removeTurret(); // removes the turret

    float getRange() const;

private:
    bool activated; // turret activated state
    int health; // turret health pool
    int ammo; // turret ammo pool
    bool boolPlaced;
    float range; // turret's range.
};

#endif
