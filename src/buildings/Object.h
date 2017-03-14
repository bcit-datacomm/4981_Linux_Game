#ifndef OBJECT_H
#define OBJECT_H
#include <string>
#include "../basic/Entity.h"
#include "../sprites/LTexture.h"
#include <SDL2/SDL.h>
#include "../collision/HitBox.h"

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
    int width; // Width of object
    int height; // Height of object
    double angle;
};

#endif
