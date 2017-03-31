#include "Marine.h"
#include "../game/GameManager.h"
#include "../log/log.h"

Marine::Marine(const int32_t id, const SDL_Rect& dest, const SDL_Rect& movementSize, const SDL_Rect& projectileSize,
        const SDL_Rect& damageSize): Entity(id, dest, movementSize, projectileSize, damageSize),
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

void Marine::collidingProjectile(const int damage) {
    health -= damage;
}

// Created by DericM 3/8/2017
void Marine::fireWeapon() {
    Weapon *w = inventory.getCurrent();
    if(w) {
        w->fire(*this);
    } else {
        logv("Slot Empty\n");
    }
}


/*
 * Created By Maitiu
 * Modified: Mar. 15 2017 - Mark Tattrie
 * Description: Checks The pick up Hitboxes of the Weapon Drops and Turrets to see if the player's
 * Marine is touching them IF Touching a Weapon Drop it Calls the Inventory Pick up method.
 */
int32_t Marine::checkForPickUp() {
    int32_t pickId = -1;
    GameManager *gm = GameManager::instance();
    CollisionHandler& ch = gm->getCollisionHandler();

    Entity *ep = ch.detectPickUpCollision(ch.getQuadTreeEntities(ch.quadtreePickUp,this),this);
    if(ep != nullptr) {
        //get Entity drop Id
        pickId = ep->getId();
        logv("Searching for id:%d in weaponDropManager\n", pickId);
        // checks if Id matches any turret Ids in turretManager, if yes, then return with the Id
        if (gm->getTurretManager().count(pickId)) {
            return pickId;
        }
        //Checks if WeaponDrop exists
        if(gm->weaponDropExists(pickId)) {
            const WeaponDrop& wd = gm->getWeaponDrop(pickId);
            //Get Weaopn id from weapon drop
            pickId = wd.getWeaponId();

            //Picks up Weapon
            if(inventory.pickUp(pickId, wd.getX(), wd.getY())) {
                gm->deleteWeaponDrop(wd.getId());
            }
        } else {
            logv("unable to find id:%d in weaponDropManager\n", pickId);
        }
    } else {
        loge("Pick id was nullptr\n");
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
        if (getSpritePosX() == 0) {
            setSpritePosX(SPRITE_SIZE_X);   //stops lag when taking first step
        }
        else if ((int)frameCount % FRAME_COUNT_WALK == 0) {
            if (getSpritePosX() < SPRITE_SIZE_X*2) {
                setSpritePosX(getSpritePosX() + SPRITE_SIZE_X); //moves to last walking position on sprite
            } else {
                setSpritePosX(SPRITE_SIZE_X);   //moves to first walking position on sprite
            }
        } 
    }
    else if (state[SDL_SCANCODE_DOWN] || state[SDL_SCANCODE_S]) {
        if (getSpritePosX() == 0) {
            setSpritePosX(SPRITE_SIZE_X);   //stops lag when taking first step
        }
        else if ((int)frameCount % FRAME_COUNT_WALK == 0) {
            if (getSpritePosX() < SPRITE_SIZE_X*2) {
                setSpritePosX(getSpritePosX() + SPRITE_SIZE_X); //moves to last walking position on sprite
            } else {
                setSpritePosX(SPRITE_SIZE_X);   //moves to first walking position on sprite
            }
        } 
    }
    else if (state[SDL_SCANCODE_LEFT] || state[SDL_SCANCODE_A]) {
        if (getSpritePosX() == 0) {
            setSpritePosX(SPRITE_SIZE_X);   //stops lag when taking first step
        }
        else if ((int)frameCount % FRAME_COUNT_WALK == 0) {
            if (getSpritePosX() < SPRITE_SIZE_X*2) {
                setSpritePosX(getSpritePosX() + SPRITE_SIZE_X); //moves to last walking position on sprite
            } else {
                setSpritePosX(SPRITE_SIZE_X);   //moves to first walking position on sprite
            }
        } 
    }
    else if (state[SDL_SCANCODE_RIGHT] || state[SDL_SCANCODE_D]) {
        if (getSpritePosX() == 0) {
            setSpritePosX(SPRITE_SIZE_X);   //stops lag when taking first step
        }
        else if ((int)frameCount % FRAME_COUNT_WALK == 0) {
            if (getSpritePosX() < SPRITE_SIZE_X*2) {
                setSpritePosX(getSpritePosX() + SPRITE_SIZE_X); //moves to last walking position on sprite
            } else {
                setSpritePosX(SPRITE_SIZE_X);   //moves to first walking position on sprite
            }
        } 
    }
    else if (state[SDL_SCANCODE_H]) {  
        setSpritePosX(SPRITE_SIZE_X*3); //testing hit state (change to when HP delta > 0)
    }
    else if (state[SDL_SCANCODE_J]) {
        setSrcRect(SPRITE_SIZE_X*4, SPRITE_SIZE_Y*7, 110, 100); //testing dead state (change to when HP == 0)
    }
    else{
        Entity::setSpritePosX(0); //reset to normal standing position if there is not keyboard input
    }
}
