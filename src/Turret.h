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

class Turret : public Entity {

	public:
	void spawnTurret(); // spawns a DEACTIVATED turret
	
	bool placementCheckTurret(); // checks if turret placement is within bounds
	
	bool collisionCheckTurret(float x, float y, CollisionHandler* ch); // checks if the turret placement overlaps with any currently existing objects
	
	void activateTurret(); // activates the turret 
	
	void onCollision();
	
	void collidingProjectile(int damage);
	
	void damageTurret(int damage); // decrements the turrets health by damage parameter
	
	void decrementAmmo(int amount); // turret ammo pool decrements by this amount
	
	void shootTurret(); // turret shoots, this is not yet defined
	
	bool ammoCheckTurret();  // returns true if turret has >0 ammo, false otherwise
	
	bool healthCheckTurret();  // returns true if turret has >0 health, false otherwise
    
	bool targetScanTurret(); // checks if there are any enemies in the turret's coverage area
	
	void setPosition(float x, float y); // Set turret position
	
	void setX(float px); //set x coordinate

    void setY(float py); //set y coordinate
	
    void setAngle(double a);//sets angle of sprite to
	
    double getAngle(); //returns sprites angle
	
	void removeTurret(); // removes the turret
	
	Turret();
	virtual ~Turret();
	
	SDL_Rect turretSpriteClips[1];
	HitBox movementHitBox;	// Hit box for movement
	HitBox projectileHitBox; // Hit box for projectiles
	HitBox damageHitBox; // Hit box for damage
	
	private:
	bool activated = false; // turret activated state 
	double angle = 0.0; // turret direction angle in which it is pointing
	int health = 200; // turret health pool
	int ammo = 100; // turret ammo pool
	
};

#endif