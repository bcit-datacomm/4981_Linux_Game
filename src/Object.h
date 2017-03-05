#ifndef OBJECT_H
#define OBJECT_H
#include <string>
#include "Entity.h"
#include "LTexture.h"
#include <SDL2/SDL.h>
#include "HitBox.h"

class Object : public Entity {
public:
    Object(int h = 0, int w = 0, double ang = 0);
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
    int height; // Height of object
    int width; // Width of object
    double angle;
};

#endif
