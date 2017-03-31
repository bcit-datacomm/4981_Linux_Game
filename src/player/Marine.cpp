#include "Marine.h"
#include "../game/GameManager.h"
#include "../log/log.h"

Marine::Marine(int32_t id, const SDL_Rect &dest, const SDL_Rect &movementSize, const SDL_Rect &projectileSize,
        const SDL_Rect &damageSize): Entity(id, dest, movementSize, projectileSize, damageSize),
        Movable(id, dest, movementSize, projectileSize, damageSize, MARINE_VELOCITY) {
    //movementHitBox.setFriendly(true); Uncomment to allow movement through other players
    //projectileHitBox.setFriendly(true); Uncomment for no friendly fire
    //damageHitBox.setFriendly(true); Uncomment for no friendly fire
    logv("Create Marine\n");
}

Marine::~Marine() {
    logv("Destroy Marine\n");
}

void Marine::onCollision() {
    // Do nothing for now
}

void Marine::collidingProjectile(int damage) {
    health = health - damage;
}

// Created by DericM 3/8/2017
void Marine::fireWeapon() {
    Weapon* w = inventory.getCurrent();
    if( w != nullptr) {
        w->fire(*this);
    } else {
        logv("Slot Empty\n");
    }
}


int32_t Marine::checkForPickUp() {

    int32_t PickId = -1;

    CollisionHandler &ch = GameManager::instance()->getCollisionHandler();

    Entity* ep = ch.detectPickUpCollision(ch.getQuadTreeEntities(ch.quadtreePickUp,this),this);

    if(ep != nullptr) {
        const auto& tm = GameManager::instance()->getTurretManager();
        //get Entity drop Id
        PickId = ep->getId();
        // checks if Id matches any turret Ids in turretManager, if yes, then return with the Id
        const auto& it = tm.find(PickId);

        if (it != tm.end()) {
            return PickId;
        }

        const WeaponDrop &wd = GameManager::instance()->getWeaponDrop(PickId);

        //Get Weaopn id from weapon drop
        PickId = wd.getWeaponId();

        if(inventory.pickUp(PickId)) {

            GameManager::instance()->deleteWeaponDrop(wd.getId());
        }
    }
    return -1;
}

/**
* Function: Marine::updateImageDirection
*
* Date: Mar 27 
*
* Designer: Aing Ragunathan
*
* Programmer: Aing Ragunathan
*
* Interface: void Marine::updateImageDirection()
*
* Returns: void
*
* Notes:
*       This function changes the direction that the character is facing.
*       It is called from GameStateMatch::handle after every frame and
*       updates the direction of the sprite according to the angle of the
*       mouse from the center of the screen.
*/
void Marine::updateImageDirection() {
    double angle = getAngle();
    double radians = (angle -90) * M_PI/180;
    
    //order: start from ~0 rad, counter clockwise
    if (radians > -M_PI/8 && radians < M_PI/8) { 
        Entity::setSpritePosY(SPRITE_SIZE_Y*2); //right
    }
    else if (radians > -3*M_PI/8 && radians < -M_PI/8) { 
        Entity::setSpritePosY(SPRITE_SIZE_Y*3);  //back & right
    }
    else if (radians > -5*M_PI/8 && radians < -3*M_PI/8) { 
        Entity::setSpritePosY(SPRITE_SIZE_Y*4);  //back
    }
    else if (radians > -7*M_PI/8 && radians < -5*M_PI/8) { 
        Entity::setSpritePosY(SPRITE_SIZE_Y*5); //back & left
    }
    else if (radians > -9*M_PI/8 && radians < -7*M_PI/8) { 
        Entity::setSpritePosY(SPRITE_SIZE_Y*6); //left
    }
    else if (radians > -11*M_PI/8 && radians < -9*M_PI/8) { 
        Entity::setSpritePosY(SPRITE_SIZE_Y*7); //front & left
    }
    else if (radians < -11*M_PI/8) { 
        Entity::setSpritePosY(0); //front 
    }
    else if (radians > M_PI/8 && radians < 3*M_PI/8) { 
        Entity::setSpritePosY(SPRITE_SIZE_Y); //front & right
    }

    setSrcRect(Entity::getSpritePosX(), Entity::getSpritePosY(), 75, 125); //update image
}

/**
* Function: Marine::updateImageWalk
*
* Date: Mar 30 
*
* Designer: Aing Ragunathan
*
* Programmer: Aing Ragunathan
*
* Interface: void Marine::updateImageWalk(double frameCount)
*       double frameCount - counted frames while walking
*
* Returns: void
*
* Notes:
*       This function repeatedly updates the image of the marine in order to animate 
*       walking. It is called from GameStateMatch::handle after every frame and updates
*       the feet placement of the marine.
*/
void Marine::updateImageWalk(const Uint8 *state, double frameCount) {
    if (state[SDL_SCANCODE_UP] || state[SDL_SCANCODE_W]) {
        if(getSpritePosX() == 0){
            setSpritePosX(SPRITE_SIZE_X);   //stops lag when taking first step
        }
        else if((int)frameCount % FRAME_COUNT_WALK == 0){
            if(getSpritePosX() < SPRITE_SIZE_X*2){
                setSpritePosX(getSpritePosX() + SPRITE_SIZE_X); //moves to last walking position on sprite
            } else {
                setSpritePosX(SPRITE_SIZE_X);   //moves to first walking position on sprite
            }
        } 
    }
    else if (state[SDL_SCANCODE_DOWN] || state[SDL_SCANCODE_S]) {
        if(getSpritePosX() == 0){
            setSpritePosX(SPRITE_SIZE_X);   //stops lag when taking first step
        }
        else if((int)frameCount % FRAME_COUNT_WALK == 0){
            if(getSpritePosX() < SPRITE_SIZE_X*2){
                setSpritePosX(getSpritePosX() + SPRITE_SIZE_X); //moves to last walking position on sprite
            } else {
                setSpritePosX(SPRITE_SIZE_X);   //moves to first walking position on sprite
            }
        } 
    }
    else if (state[SDL_SCANCODE_LEFT] || state[SDL_SCANCODE_A]) {
        if(getSpritePosX() == 0){
            setSpritePosX(SPRITE_SIZE_X);   //stops lag when taking first step
        }
        else if((int)frameCount % FRAME_COUNT_WALK == 0){
            if(getSpritePosX() < SPRITE_SIZE_X*2){
                setSpritePosX(getSpritePosX() + SPRITE_SIZE_X); //moves to last walking position on sprite
            } else {
                setSpritePosX(SPRITE_SIZE_X);   //moves to first walking position on sprite
            }
        } 
    }
    else if (state[SDL_SCANCODE_RIGHT] || state[SDL_SCANCODE_D]) {
        if(getSpritePosX() == 0){
            setSpritePosX(SPRITE_SIZE_X);   //stops lag when taking first step
        }
        else if((int)frameCount % FRAME_COUNT_WALK == 0){
            if(getSpritePosX() < SPRITE_SIZE_X*2){
                setSpritePosX(getSpritePosX() + SPRITE_SIZE_X); //moves to last walking position on sprite
            } else {
                setSpritePosX(SPRITE_SIZE_X);   //moves to first walking position on sprite
            }
        } 
    }
    else if (state[SDL_SCANCODE_H]) {  
        setSpritePosX(SPRITE_SIZE_X*3); //testing hit state (change to when HP delta > 0)
    }
    else if (state[SDL_SCANCODE_J]){
        setSrcRect(SPRITE_SIZE_X*4, SPRITE_SIZE_Y*7, 110, 100); //testing dead state (change to when HP == 0)
    }
    else{
        Entity::setSpritePosX(0); //reset to normal standing position if there is not keyboard input
    }
}
