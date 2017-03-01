#ifndef MOVABLE_H
#define MOVABLE_H
#include "Entity.h"
#include "CollisionHandler.h"

class Movable : public virtual Entity {
public:
    Movable(int vel) : velocity(vel) {};
    virtual ~Movable() {};

    void move(float moveX, float moveY, CollisionHandler* ch); // Moves Marine

    void setDX(float px); //set delta x coordinate

    void setDY(float py); //set delta y coordinate

    void setVelocity(int pvel); // set velocity of Marine movement

    float getDX(); // get delta x coordinate

    float getDY(); //get delta y coordinate

    int	getVelocity(); // get velocity of Marine movement

    void setAngle(double a);//sets angle of sprite to

    double getAngle(); //returns sprites angle
private:
    float dx = 0; // delta x coordinat
    float dy = 0; //delta ycoordinate
    double angle = 0.0;
    int velocity = 500; // velocity of Marine movement
};

#endif
