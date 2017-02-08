// Created 05/02/2017 Mark C.
#include "Turret.h"

Turret::Turret() {
    turretSpriteClips[0].x = 0;
	turretSpriteClips[0].y = 0;
	turretSpriteClips[0].w = 100;
	turretSpriteClips[0].h = 100;
	this->movementHitBox.setRect(turretSpriteClips[0]);
	this->movementHitBox.move(this->getX(), this->getY());
	this->movementHitBox.attached = this;
	//this->movementHitBox.setFriendly(true); Uncomment to allow movement through other players
	this->projectileHitBox.setRect(turretSpriteClips[0]);
	this->projectileHitBox.move(this->getX(), this->getY());
	this->projectileHitBox.attached = this;
	//this->projectileHitBox.setFriendly(true); Uncomment for no friendly fire
	this->damageHitBox.setRect(turretSpriteClips[0]);
	this->damageHitBox.move(this->getX(), this->getY());
	this->damageHitBox.attached = this;
	//this->damageHitBox.setFriendly(true); Uncomment for no friendly fire
}

Turret::~Turret() {
	printf("Destroy Turret\n");
}

// checks if turret placement is within bounds, currently does nothing
bool Turret::placementCheckTurret(){
    return true;
}
	
// checks if the turret placement overlaps with any currently existing objects, currently does nothing
bool Turret::collisionCheckTurret(float x, float y, CollisionHandler* ch) {
    SDL_Rect checkBox;
    checkBox.h = 100;
    checkBox.w = 100;
    checkBox.x = x;
    checkBox.y = y;
	HitBox hitBox;
    hitBox.setRect(checkBox);
    if(ch->detectMovementCollision(&hitBox))
        return false;
    return true;
}

// activates the turret     
void Turret::activateTurret() {
    this->activated = true;
}
	
void Turret::onCollision() {
    // Does nothing for now
}
	
void Turret::collidingProjectile(int damage) {
    this->health = health - damage;    
}      

// turret ammo pool decrements by this amount    
void Turret::decrementAmmo(int amount) {
    this->ammo = ammo- amount;
}

// turret shoots, this is not yet defined    
void Turret::shootTurret() {

}

 // returns true if turret has >0 ammo, false otherwise
bool Turret::ammoCheckTurret() {
    if (ammo > 0)
        return true;
    else
        return false;
}
	
 // returns true if turret has >=1 health, false otherwise  
bool Turret::healthCheckTurret() {
    if (health > 0) 
        return true;
    else
        return false;
}

// checks if there are any enemies in the turret's coverage area, this is not yet defined
bool Turret::targetScanTurret() {
    return true;
}
	
// Set turret position    
void Turret::setPosition(float x, float y) {
    Entity::setPosition(x, y);
	this->movementHitBox.move(this->getX(), this->getY());
	this->projectileHitBox.move(this->getX(), this->getY());
}

//set x coordinate    
void Turret::setX(float px) {
    Entity::setX(px);
	this->movementHitBox.move(this->getX(), this->getY());
	this->projectileHitBox.move(this->getX(), this->getY());
}

//set y coordinate    
void Turret::setY(float py) {
    Entity::setY(py);
	this->movementHitBox.move(this->getX(), this->getY());
	this->projectileHitBox.move(this->getX(), this->getY());
}
	
//sets angle of sprite to    
void Turret::setAngle(double a) {
    angle = a;
}
	
//returns sprites angle, currently does nothing
double Turret::getAngle() {
    return angle;
}