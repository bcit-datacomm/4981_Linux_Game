/*------------------------------------------------------------------------------
 * Header: Movable.h   
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
#ifndef MOVABLE_H
#define MOVABLE_H
#include "Entity.h"
#include "../collision/CollisionHandler.h"

constexpr double THREE_HUNDRED_SIXTY_DEGREES = 360.0;
class Movable : public virtual Entity {
public:
    //for Marines and Zombies
    Movable(const int32_t id, const SDL_Rect& dest, const SDL_Rect& movementSize, const SDL_Rect& projectileSize,
        const SDL_Rect& damageSize, const int vel) : Entity(id, dest, movementSize, projectileSize,
        damageSize), velocity(vel), dx(0), dy(0), angle(0.0) {};

    //for turrets
    Movable(const int32_t id, const SDL_Rect& dest, const SDL_Rect& movementSize, const SDL_Rect& projectileSize,
        const SDL_Rect& damageSize, const SDL_Rect& pickupSize, const int vel) : Entity(id, dest, movementSize,
        projectileSize, damageSize, pickupSize), velocity(vel), dx(0), dy(0), angle(0.0) {};

    virtual ~Movable() = default;
    // Moves Marine
    void move(const float moveX, const float moveY, CollisionHandler& ch);
    //set delta x coordinate
    void setDX(const float px) {dx = px;}
    //set delta y coordinate
    void setDY(const float py) {dy = py;}
    // set velocity of Marine movement
    void setVelocity(int pvel) {velocity = pvel;}
    //sets angle of sprite in degrees
    void setAngle(const double a) {angle = fmod(a, THREE_HUNDRED_SIXTY_DEGREES);}
    //sets the angle of the sprite in degrees
    void setRadianAngle(const double a) {setAngle(a * 180 / M_PI);}
    // get delta x coordinate
    auto getDX() const {return dx;}
    //get delta y coordinate
    auto getDY() const {return dy;}
    // get velocity of Marine movement
    auto getVelocity() const {return velocity;} 
    //returns sprites angle in degrees
    double getAngle() const {return angle;}
    //returns the sprites angle in radians
    auto getRadianAngle() const {return (angle) * M_PI / 180;}
private:
    int velocity; // velocity of object
    float dx;     // delta x coordinat
    float dy;     // delta ycoordinate
    double angle; // moving angle
};

#endif
