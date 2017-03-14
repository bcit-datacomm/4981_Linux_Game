#ifndef MOVABLE_H
#define MOVABLE_H
#include "Entity.h"
#include "CollisionHandler.h"

class Movable : public virtual Entity {
public:
    Movable(int vel) : dx(0), dy(0), velocity(vel), angle(0.0) {};
    virtual ~Movable() {};

    void move(float moveX, float moveY, CollisionHandler& ch); // Moves Marine

    void setDX(float px); //set delta x coordinate

    void setDY(float py); //set delta y coordinate

    void setVelocity(int pvel); // set velocity of Marine movement

    float getDX() const; // get delta x coordinate

    float getDY() const; //get delta y coordinate

    int getVelocity() const; // get velocity of Marine movement

    void setAngle(double a);//sets angle of sprite to

    double getAngle() const; //returns sprites angle
private:
    float dx;     // delta x coordinat
    float dy;     // delta ycoordinate
    int velocity; // velocity of object
    double angle; // moving angle
};

#endif
