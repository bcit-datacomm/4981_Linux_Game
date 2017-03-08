#ifndef ENTITY_H
#define ENTITY_H
#include <string>
#include <memory>
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
	
	void updateHitBoxes(); // update hitbox positions

    std::shared_ptr<HitBox> movementHitBox = nullptr;
	std::shared_ptr<HitBox> projectileHitBox = nullptr;
	std::shared_ptr<HitBox> damageHitBox = nullptr;
	std::shared_ptr<HitBox> pickupHitBox = nullptr;

    LTexture texture;
    SDL_Rect spriteClips[1];

private:

    float x = 0; //x coordinate
    float y = 0; //y coordinate

};

#endif
