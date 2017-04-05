/*------------------------------------------------------------------------------
* Source: Entity.cpp    
*
* Functions:
*    
*
* Date: Feb. 4, 2017
*
* Revisions: 
* Edited By : Yiaoping Shu- Style guide
*
* Designer: Isaac Morneau + Jacob McPhail 
*
* Author: Isaac Morneau + Jacob McPhail 
*
* Notes:
*  
------------------------------------------------------------------------------*/
#include "Entity.h"
#include <atomic>
#include <cstdint>
#include "../log/log.h"

/**
 * Date: Feb. 4, 2017
 * Modified: Jacob McPhail (Init Creation)
 * Author:  ----
 * Function Interface: Entity(const int32_t nid, const SDL_Rect& dest)
 *      nid : 
 *      dest: 
 *
 * Description:
 *      ctor for entity
 */
Entity::Entity(const int32_t nid, const SDL_Rect& dest):id(nid), destRect(dest), srcRect({0,0, dest.w, dest.h}),
        movementHitBox(dest), projectileHitBox(dest), damageHitBox(dest), pickupHitBox(dest),
        x(dest.x), y(dest.y) {
}

Entity::Entity(const int32_t nid, const SDL_Rect& dest, const SDL_Rect& movementSize): id(nid), destRect(dest),
        srcRect({0,0, dest.w, dest.h}), movementHitBox(movementSize), projectileHitBox(dest),
        damageHitBox(dest), pickupHitBox(dest), x(dest.x), y(dest.y) {
}

//Weapon drops
Entity::Entity(const int32_t nid, const SDL_Rect& dest, const SDL_Rect& movementSize,
        const SDL_Rect &pickupSize): id(nid), destRect(dest), srcRect({0,0, dest.w, dest.h}),
        movementHitBox(movementSize), projectileHitBox(dest), damageHitBox(dest),pickupHitBox(pickupSize),
        x(dest.x), y(dest.y) {
}

Entity::Entity(const int32_t nid, const SDL_Rect& dest, const SDL_Rect& movementSize,
        const SDL_Rect& projectileSize, const SDL_Rect& damageSize): id(nid), destRect(dest),
        srcRect({0,0, dest.w, dest.h}), movementHitBox(movementSize), projectileHitBox(projectileSize),
        damageHitBox(damageSize), pickupHitBox(dest), x(dest.x), y(dest.y) {
}


//movables and marines
Entity::Entity(const int32_t nid, const SDL_Rect& dest, const SDL_Rect& movementSize,
        const SDL_Rect& projectileSize, const SDL_Rect& damageSize, const SDL_Rect& pickupSize):
        id(nid), destRect(dest), srcRect({0,0, dest.w, dest.h}), movementHitBox(movementSize),
        projectileHitBox(projectileSize), damageHitBox(damageSize), pickupHitBox(pickupSize),
        x(dest.x), y(dest.y) {
}

Entity::Entity(const Entity& e): id(e.id), destRect(e.destRect), srcRect(e.srcRect),
        movementHitBox(e.movementHitBox), projectileHitBox(e.projectileHitBox),
        damageHitBox(e.damageHitBox), pickupHitBox(e.pickupHitBox), x(e.x), y(e.y) {
}

const SDL_Rect Entity::getRelativeDestRect(const SDL_Rect& view) const {
    return {destRect.x - view.x , destRect.y - view.y, static_cast<int>(destRect.w), static_cast<int>(destRect.h)};
}

/**
 * Date: Feb. 4, 2017
 * Modified: ----
 * Author: Jacob McPhail.
 * Function Interface: 
 * Description: setX(const float px)
 *      px : new x position
 *
 *      Set x coordinate
 */
void Entity::setX(const float px) {
    x = px;
    destRect.x = px;
    updateHitBoxes();
}

/**
 * Date: Feb. 4, 2017
 * Modified: ----
 * Author: Jacob McPhail.
 * Function Interface: setY(const float py)
 *      py : new y position
 *
 * Description:
 *      Set y coordinate
 */
void Entity::setY(const float py) {
    y = py;
    destRect.y = py;
    updateHitBoxes();
}

/**
 * Date: Feb. 4, 2017
 * Modified: ----
 * Author: Jacob McPhail.
 * Function Interface: setPosition(float px, float py)
 *      px : new x position
 *      py : new y position
 *
 * Description:
 *      Set Entity by x and y amount
 */
void Entity::setPosition(float px, float py) {
    x = px;
    y = py;
    destRect.x = px;
    destRect.y = py;
    updateHitBoxes();
}

/**
 * Date: Feb. 9, 2017
 * Modified: ----
 * Author: Jacob McPhail
 * Function Interface: updateHitBoxes() 
 * Description:
 *     Updates the hit boxes
 */
void Entity::updateHitBoxes() {
    movementHitBox.move(x, y);
    projectileHitBox.move(x, y);
    damageHitBox.move(x, y);
    pickupHitBox.move(x - 10, y - 10);
}

/**
 * Date: Feb. 9, 2017
 * Modified: ----
 * Author: Jacob McPhail
 * Function Interface: updateRectHitBoxes() 
 * Description:
 *     Updates the rectangle for the hitbox
 */
void Entity::updateRectHitBoxes() {
    movementHitBox.setRect(destRect);
    projectileHitBox.setRect(destRect);
    damageHitBox.setRect(destRect);
    pickupHitBox.setRect(destRect);
}

/**
 * Date: Feb. 4, 2017
 * Modified: ----
 * Author: Jacob McPhail
 * Function Interface: onCollision()
 */
void Entity::onCollision() {
    //do nothing
}

/**
 * Date: Feb. 4, 2017
 * Modified: ----
 * Author: Jacob McPhail
 * Function Interface: collidingProjectile(const int damage) 
 */
void Entity::collidingProjectile(const int damage) {
    //do nothing
}

//sets the destination rect of where to render stuff
void Entity::setDestRect(const int x, const int y, const int width, const int height) {
    destRect.x = x;
    destRect.y = y;
    destRect.w = width;
    destRect.h = height;
}

//sets the source rect, used for sprites and textures
void Entity::setSrcRect(const int x, const int y, const int width, const int height) {
    srcRect.x = x;
    srcRect.y = y;
    srcRect.w = width;
    srcRect.h = height;
}
