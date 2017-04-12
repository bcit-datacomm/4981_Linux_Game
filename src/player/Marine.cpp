#include "Marine.h"
#include <cstdlib>
#include "../game/GameManager.h"
#include "../log/log.h"

/**
 * Date: Feb. 4, 2017
 * Modified: ----
 * Author: Jacob McPhail
 * Function Interface: Marine(const int32_t id, const SDL_Rect& dest,
 *              const SDL_Rect& movementSize, const SDL_Rect& projectileSize, const SDL_Rect& damageSize)
 *
 *              id : Marine id
 *              dest : Destination rect
 *              movmentSize : Move hitbox size
 *              projectileSize : Projectile hitbox size
 *              damageSize : Damage hitbox size
 *
 * Description:
 *     ctor for a marine.
 */
Marine::Marine(const int32_t id, const SDL_Rect& dest, const SDL_Rect& movementSize,
        const SDL_Rect& projectileSize, const SDL_Rect& damageSize)
: Entity(id, dest, movementSize, projectileSize, damageSize),
        Movable(id, dest, movementSize, projectileSize, damageSize, MARINE_VELOCITY) {
    logv("Create Marine\n");
}

/**
 * Date: Feb. 4, 2017
 * Modified: ----
 * Author: Jacob McPhail
 * Function Interface: ~Marine()
 * Description:
 *   dctor for a marine.
 */
Marine::~Marine() {
    logv("Destroy Marine\n");
}

/**
 * Date: Feb. 4, 2017
 * Modified: ----
 * Author: Jacob McPhail
 * Function Interface: onCollision()
 */
void Marine::onCollision() {
    // Do nothing for now
}

/**
 * Date: Feb. 4, 2017
 * Modified: ----
 * Author: Jacob McPhail
 * Function Interface: collidingProjectile(const int damage)
 *      damage : damage to deal to marine
 *
 * Description:
 *     Deals damage to a marine.
 */
void Marine::collidingProjectile(const int damage) {
    health -= damage;
}

// Created by DericM 3/8/2017
bool Marine::fireWeapon() {
    Weapon *w = inventory.getCurrent();
    if(w) {
        return w->fire(*this);
    } else {
        logv("Slot Empty\n");
        return false;
    }
}

/**
* Date: Mar 27
* Author: Aing Ragunathan
*
* Interface: void Marine::updateImageDirection()
*
* Description:
*       This function changes the direction that the character is facing.
*       It is called from GameStateMatch::handle after every frame and
*       updates the direction of the sprite according to the angle of the
*       mouse from the center of the screen.
*/
void Marine::updateImageDirection() {
    const double radians = getRadianAngle() - M_PI / 2;

    //order: start from ~0 rad, counter clockwise
    if (radians > SPRITE_ANGLE2 && radians < SPRITE_ANGLE1) {
        setSrcRect(getSrcRect().x, SPRITE_RIGHT, SPRITE_SIZE_X, SPRITE_SIZE_Y);
    } else if (radians > SPRITE_ANGLE3 && radians < SPRITE_ANGLE2) {
        setSrcRect(getSrcRect().x, SPRITE_BACK_RIGHT, SPRITE_SIZE_X, SPRITE_SIZE_Y);
    } else if (radians > SPRITE_ANGLE4 && radians < SPRITE_ANGLE3) {
        setSrcRect(getSrcRect().x, SPRITE_BACK, SPRITE_SIZE_X, SPRITE_SIZE_Y);
    } else if (radians > SPRITE_ANGLE5 && radians < SPRITE_ANGLE4) {
        setSrcRect(getSrcRect().x, SPRITE_BACK_LEFT, SPRITE_SIZE_X, SPRITE_SIZE_Y);
    } else if (radians > SPRITE_ANGLE6 && radians < SPRITE_ANGLE5) {
        setSrcRect(getSrcRect().x, SPRITE_LEFT, SPRITE_SIZE_X, SPRITE_SIZE_Y);
    } else if (radians > SPRITE_ANGLE7 && radians < SPRITE_ANGLE6) {
        setSrcRect(getSrcRect().x, SPRITE_FRONT_LEFT, SPRITE_SIZE_X, SPRITE_SIZE_Y);
    } else if (radians < SPRITE_ANGLE7) {
        setSrcRect(getSrcRect().x, SPRITE_FRONT, SPRITE_SIZE_X, SPRITE_SIZE_Y);
    } else if (radians > SPRITE_ANGLE1 && radians < SPRITE_ANGLE8) {
        setSrcRect(getSrcRect().x, SPRITE_FRONT_RIGHT, SPRITE_SIZE_X, SPRITE_SIZE_Y);
    }
}

/**
* Date: Mar 30
* Modified: April 4 (Brody McCrone) - Swapped use of keyboard input to deltas.
*
* Author: Aing Ragunathan
*
* Function Interface: void Marine::updateImageWalk(double frameCount)
*       double frameCount - counted frames while walking
*
* Description:
*       This function repeatedly updates the image of the marine in order to animate
*       walking. It is called from GameManager::updateMarines every frame.
*/
void Marine::updateImageWalk() {
    static unsigned long frameCount = 0;
    ++frameCount;
    const float dy = getDY();
    const float dx = getDX();
    if (dy > 0) {
        //stops lag when taking first step
        if (getSrcRect().x == SPRITE_FRONT) {
            setSrcRect(SPRITE_SIZE_X, getSrcRect().y, SPRITE_SIZE_X, SPRITE_SIZE_Y);
        } else if (frameCount % FRAME_COUNT_WALK == 0) {
            //cycle throught the walking images
            if (getSrcRect().x < SPRITE_NEXT_STEP) {
                setSrcRect(getSrcRect().x + SPRITE_SIZE_X, getSrcRect().y, SPRITE_SIZE_X, SPRITE_SIZE_Y);
            } else {
                setSrcRect(SPRITE_SIZE_X, getSrcRect().y, SPRITE_SIZE_X, SPRITE_SIZE_Y);
            }
        }
    } else if (dy < 0) {
        if (getSrcRect().x == SPRITE_FRONT) {
            setSrcRect(SPRITE_SIZE_X, getSrcRect().y, SPRITE_SIZE_X, SPRITE_SIZE_Y);
        } else if (frameCount % FRAME_COUNT_WALK == 0) {
            if (getSrcRect().x < SPRITE_NEXT_STEP) {
                setSrcRect(getSrcRect().x + SPRITE_SIZE_X, getSrcRect().y, SPRITE_SIZE_X, SPRITE_SIZE_Y);
            } else {
                setSrcRect(SPRITE_SIZE_X, getSrcRect().y, SPRITE_SIZE_X, SPRITE_SIZE_Y);
            }
        }
    } else if (dx < 0) {
        if (getSrcRect().x == SPRITE_FRONT) {
            setSrcRect(SPRITE_SIZE_X, getSrcRect().y, SPRITE_SIZE_X, SPRITE_SIZE_Y);
        } else if (frameCount % FRAME_COUNT_WALK == 0) {
            if (getSrcRect().x < SPRITE_NEXT_STEP) {
                setSrcRect(getSrcRect().x + SPRITE_SIZE_X, getSrcRect().y, SPRITE_SIZE_X, SPRITE_SIZE_Y);
            } else {
                setSrcRect(SPRITE_SIZE_X, getSrcRect().y, SPRITE_SIZE_X, SPRITE_SIZE_Y);
            }
        }
    } else if (dx > 0) {
        if (getSrcRect().x == SPRITE_FRONT) {
            setSrcRect(SPRITE_SIZE_X, getSrcRect().y, SPRITE_SIZE_X, SPRITE_SIZE_Y);
        } else if (frameCount % FRAME_COUNT_WALK == 0) {
            if (getSrcRect().x < SPRITE_NEXT_STEP) {
                setSrcRect(getSrcRect().x +SPRITE_SIZE_X, getSrcRect().y, SPRITE_SIZE_X, SPRITE_SIZE_Y);
            } else {
                setSrcRect(SPRITE_SIZE_X, getSrcRect().y, SPRITE_SIZE_X, SPRITE_SIZE_Y);
            }
        }
    } else {
        setSrcRect(SPRITE_FRONT, getSrcRect().y, SPRITE_SIZE_X, SPRITE_SIZE_Y);
    }
}


void Marine::updateLifeState() {
    if (!lifeState && static_cast<int>(SDL_GetTicks()) >= (respawnTick + RESPAWN_DELAY)) {
        lifeState = true;
        health = MARINE_MAX_HEALTH;
        const auto& spawnPoint = GameManager::instance()->getBase().getSpawnPoint();
        setPosition(spawnPoint.first, spawnPoint.second);
    } else if (lifeState && health <= 0) {
        lifeState = false;
        respawnTick = SDL_GetTicks();
        setPosition(HELL_ZONE, HELL_ZONE);
    }
}
