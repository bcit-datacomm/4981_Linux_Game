#include <cmath>
#include "Movable.h"

// Move Movable by x and y amount
/**
 * Date: Mar. 1, 2017
 * Modified: Mar. 15 2017 - Mark Tattrie
 * Author: Jacob McPhail
 * Function Interface: void Movable::move(float moveX, float moveY, CollisionHandler& ch)
 * Description:
 * move the movable object by moveX, moveY. If there is a collision detection, move it back
 */
void Movable::move(float moveX, float moveY, CollisionHandler& ch){
    //Move the Movable left or right
    setX(getX() + moveX);

    //if there is a collision with anything with a movement hitbox, move it back
    if (ch.detectMovementCollision(ch.getQuadTreeEntities(ch.quadtreeMarine,this),this)
            || ch.detectMovementCollision(ch.getQuadTreeEntities(ch.quadtreeZombie,this),this)
            || ch.detectMovementCollision(ch.getQuadTreeEntities(ch.quadtreeWall,this),this)
            || ch.detectMovementCollision(ch.getQuadTreeEntities(ch.quadtreeBarricade,this),this)
            || ch.detectMovementCollision(ch.getQuadTreeEntities(ch.quadtreeTurret,this),this)
            || ch.detectMovementCollision(ch.getQuadTreeEntities(ch.quadtreeObj,this),this)
            || ch.detectMovementCollision(ch.getQuadTreeEntities(ch.quadtreeStore,this),this)) {
        setX(getX() - moveX);
    }

    //Move the Movable up or down
    setY(getY()+moveY);

    //if there is a collision with anything with a movement hitbox, move it back
    if (ch.detectMovementCollision(ch.getQuadTreeEntities(ch.quadtreeMarine,this),this)
            || ch.detectMovementCollision(ch.getQuadTreeEntities(ch.quadtreeZombie,this),this)
            || ch.detectMovementCollision(ch.getQuadTreeEntities(ch.quadtreeWall,this),this)
            || ch.detectMovementCollision(ch.getQuadTreeEntities(ch.quadtreeBarricade,this),this)
            || ch.detectMovementCollision(ch.getQuadTreeEntities(ch.quadtreeTurret,this),this)
            || ch.detectMovementCollision(ch.getQuadTreeEntities(ch.quadtreeObj,this),this)
            || ch.detectMovementCollision(ch.getQuadTreeEntities(ch.quadtreeStore,this),this)) {
        setY(getY() - moveY);
    }

}

// Set delta x coordinate
void Movable::setDX(float px) {
    dx = px;
}

// Set delta y coordinate
void Movable::setDY(float py) {
    dy = py;
}

// set velocity of Movable movement
void Movable::setVelocity(int pvel) {
    velocity = pvel;
}

// Get delta x coordinate
float Movable::getDX() const{
    return dx;
}

// Get delta y coordinate
float Movable::getDY() const{
    return dy;
}

// Get velocity of Movable movement
int Movable::getVelocity() const{
    return velocity;
}

//sets the angle of the player's Movable sprite
void Movable::setAngle(double a){
    angle = fmod(a, 360);
}

//returns the angle of the player's Movable sprite
double Movable::getAngle() const{
    return angle;
}
