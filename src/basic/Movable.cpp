/*------------------------------------------------------------------------------
* Source: Movable.cpp    
*
* Functions:
*    
*
* Date: 
*
* Revisions: 
* Edited By : Yiaoping Shu- Style guide
*
* Designer: 
*
* Author: 
*
* Notes:
*  
------------------------------------------------------------------------------*/
#include <cmath>
#include "Movable.h"
#include <cmath>

// Move Movable by x and y amount
/**
 * Date: Mar. 1, 2017
 * Modified: Mar. 15 2017 - Mark Tattrie
 * Author: Jacob McPhail
 * Function Interface: void Movable::move(float moveX, float moveY, CollisionHandler& ch)
 * Description:
 * move the movable object by moveX, moveY. If there is a collision detection, move it back
 */
void Movable::move(const float moveX, const float moveY, CollisionHandler& ch){
    //Move the Movable left or right
    setX(getX() + moveX);

    //if there is a collision with anything with a movement hitbox, move it back
    if (ch.detectMovementCollision(ch.getQuadTreeEntities(ch.getZombieMovementTree(),this),this)
            || ch.detectMovementCollision(ch.getQuadTreeEntities(ch.getZombieTree(),this),this)) {
        setX(getX() - moveX);
    }

    //Move the Movable up or down
    setY(getY()+moveY);

    //if there is a collision with anything with a movement hitbox, move it back
    if (ch.detectMovementCollision(ch.getQuadTreeEntities(ch.getZombieMovementTree(),this),this)
            || ch.detectMovementCollision(ch.getQuadTreeEntities(ch.getZombieTree(),this),this)) {
        setY(getY() - moveY);
    }
}
