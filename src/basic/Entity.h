#ifndef ENTITY_H
#define ENTITY_H
#include <string>
#include <memory>
#include "../collision/HitBox.h"
#include "../sprites/LTexture.h"
#include <SDL2/SDL.h>

class Entity {
public:
    Entity();
    Entity(const Entity &e);
    virtual ~Entity();
    virtual void onCollision();
    virtual void collidingProjectile(const int damage);
    void setPosition(float x, float y); // Set marine position
    void setX(float px); //set x coordinate
    void setY(float py); //set y coordinate
    float getX() const; // get x coordinate
    float getY() const; // get y coordinate
    void updateHitBoxes(); // update hitbox positions
    void updateRectHitBoxes(); // update hitbox sizes
    int32_t getId()const{return id;}; //returns the id of the entity
    void setId(const int32_t num){id = num;};//Sets the ID, ONLY USE SPARINGLY



    LTexture texture;
    SDL_Rect spriteClips[1];
    std::shared_ptr<HitBox> movementHitBox;
    std::shared_ptr<HitBox> projectileHitBox;
    std::shared_ptr<HitBox> damageHitBox;
    std::shared_ptr<HitBox> pickupHitBox;

private:


    int32_t id; //is the index num of the entity in its respective manager
    float x = 0; //x coordinate
    float y = 0; //y coordinate

};

int32_t generateID();

#endif
