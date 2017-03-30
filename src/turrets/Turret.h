// Created 05/02/2017 Mark C.
#ifndef TURRET_H
#define TURRET_H

#include <vector>
#include <SDL2/SDL.h>

#include "../collision/HitBox.h"
#include "../basic/Entity.h"
#include "../player/Marine.h"
#include "../collision/CollisionHandler.h"
#include "../view/Window.h"

constexpr static int TURRET_HEIGHT = 100;
constexpr static int TURRET_WIDTH = 100;
constexpr static int PASS_ALPHA = 200;
constexpr static int FAIL_ALPHA = 30;
constexpr static int PLACED_ALPHA = 255;
constexpr static int PLACE_DISTANCE = 200;

class Turret : public Movable {
public:

    Turret(const int32_t id, const SDL_Rect& dest,const SDL_Rect& movementSize, const SDL_Rect& projectileSize,
        const SDL_Rect& damageSize, const SDL_Rect& pickupSize, const bool activated = false,
        const int health = 200, const int ammo = 100, const bool placed = false, const float range = 400.0f);

    virtual ~Turret();

    void spawnTurret(); // spawns a DEACTIVATED turret
    bool placementCheckTurret(); // checks if turret placement is within bounds
    // checks if the turret placement overlaps with any currently existing objects
    bool collisionCheckTurret(const float , const float , const float , const float , CollisionHandler&);
    void activateTurret(); // activates the turret
    void collidingProjectile(const int damage);
    void damageTurret(const int damage); // decrements the turrets health by damage parameter
    void shootTurret(); // turret shoots, this is not yet defined
    bool ammoCheckTurret();  // returns true if turret has > 0 ammo, false otherwise
    // Mark Chen, 2017-03-01
    bool healthCheckTurret() {return health > 0;};  // returns true if turret has >0 health, false otherwise
    bool targetScanTurret(); // checks if there are any enemies in the turret's coverage
    void move(const float playerX, const float playerY,
            const float moveX, const float moveY, CollisionHandler& ch);
    void placeTurret();
    bool isPlaced() {return placed;};
    void pickUpTurret();
    void removeTurret(); // removes the turret
    // Jamie Lee, 2017-03-01.
    float getRange() const {
        return range;
    }

private:
    bool activated; // turret activated state
    int health; // turret health pool
    int ammo; // turret ammo pool
    bool placed;
    float range; // turret's range.
};

#endif
