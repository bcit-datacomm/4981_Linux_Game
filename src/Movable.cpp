#include "Movable.h"

// Move Movable by x and y amount
void Movable::move(float moveX, float moveY, CollisionHandler &ch){
    //Move the Movable left or right
    setX(getX() + moveX);

    if (ch.detectMovementCollision(movementHitBox.get())) {
        setX(getX() - moveX);
    }

    //Move the Movable up or down
    setY(getY()+moveY);

    if (ch.detectMovementCollision(movementHitBox.get())) {
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
    angle = a;
}

//returns the angle of the player's Movable sprite
double Movable::getAngle() const{
    return angle;
}
