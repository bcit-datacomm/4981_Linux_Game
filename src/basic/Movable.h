#ifndef MOVABLE_H
#define MOVABLE_H
#include "Entity.h"
#include "../collision/CollisionHandler.h"

class Movable : public virtual Entity {
public:
    //for Marines and Zombies
    Movable(int32_t id, const SDL_Rect dest, const SDL_Rect &movementSize, const SDL_Rect &projectileSize,
            const SDL_Rect &damageSize, int vel) : Entity(id, dest, movementSize, projectileSize,
            damageSize), velocity(vel), dx(0), dy(0), angle(0.0) {};

    //for turrets
    Movable(int32_t id, const SDL_Rect dest, const SDL_Rect &movementSize, const SDL_Rect &projectileSize,
            const SDL_Rect &damageSize, const SDL_Rect &pickupSize, int vel) : Entity(id, dest, movementSize,
            projectileSize, damageSize, pickupSize), velocity(vel), dx(0), dy(0), angle(0.0) {};

    virtual ~Movable() = default;

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
    int velocity; // velocity of object
    float dx;     // delta x coordinat
    float dy;     // delta ycoordinate
    double angle; // moving angle
};

#endif
