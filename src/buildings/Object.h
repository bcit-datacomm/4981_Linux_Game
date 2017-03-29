#ifndef OBJECT_H
#define OBJECT_H

#include <string>
#include <SDL2/SDL.h>

#include "../basic/Entity.h"
#include "../collision/HitBox.h"

class Object : public Entity {
public:

    Object(const int32_t nid, const SDL_Rect& dest, const int width = 0, const int height = 0, const
            double angle = 0);

    Object(const int32_t nid, const SDL_Rect& dest, const SDL_Rect& movementSize, const SDL_Rect& pickupSize,
            const int width = 0, const int heigth = 0, const double angle = 0);
    virtual ~Object();

    void setHeight(int);
    void setWidth(int);
    void setAngle(double a);//sets angle of sprite to
    int getHeight();
    int getWidth();
    double getAngle(); //returns sprites angle
    void onCollision();
    void collidingProjectile(int damage){};

private:
    int width; // Width of object
    int height; // Height of object
    double angle;
};

#endif
