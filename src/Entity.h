#ifndef ENTITY_H
#define ENTITY_H
#include <string>
#include "HitBox.h"
#include "LTexture.h"
#include <SDL2/SDL.h>

class Entity {
public:
    Entity();
    virtual ~Entity();

    virtual void onCollision() = 0;
    virtual void collidingProjectile(int damage) = 0;

    void setPosition(float x, float y); // Set marine position

    void setX(float px); //set x coordinate

    void setY(float py); //set y coordinate

    float getX() const; // get x coordinate

    float getY() const; // get y coordinate

    HitBox movementHitBox;    // Hit box for movement
    HitBox projectileHitBox; // Hit box for projectiles
    HitBox damageHitBox; // Hit box for damage
    LTexture texture;
    SDL_Rect spriteClips[1];

private:
    float x = 200; //x coordinate
    float y = 200; //y coordinate
};

#endif
