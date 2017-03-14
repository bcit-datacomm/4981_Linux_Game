#ifndef ENTITY_H
#define ENTITY_H
#include <string>
#include <memory>
#include "HitBox.h"
#include "LTexture.h"
#include <SDL2/SDL.h>

class Entity {
public:
    //all values the same
    Entity(int32_t nid, const SDL_Rect &spriteSize);
    //size and movement different the rest are sprite size
    Entity(int32_t nid, const SDL_Rect &spriteSize, const SDL_Rect &movementSize);
    //size movement, and pickup different size, the rest are sprite size
    Entity(int32_t nid, const SDL_Rect &spriteSize, const SDL_Rect &movementSize,
        const SDL_Rect &pickupSize);
    //pickup is the same as sprite size
    Entity(int32_t nid, const SDL_Rect &spriteSize, const SDL_Rect &movementSize,
        const SDL_Rect &projectileSize, const SDL_Rect &damageSize);
    //all values are different
    Entity(int32_t nid, const SDL_Rect &spriteSize, const SDL_Rect &movementSize,
        const SDL_Rect &projectileSize, const SDL_Rect &damageSize, const SDL_Rect &pickupSize);

    Entity(const Entity &e);
    virtual ~Entity();
    virtual void onCollision();
    virtual void collidingProjectile(int damage);
    void setPosition(float x, float y); // Set marine position
    void setX(float px); //set x coordinate
    void setY(float py); //set y coordinate
    float getX() const; // get x coordinate
    float getY() const; // get y coordinate
    void updateHitBoxes(); // update hitbox positions
    void updateRectHitBoxes(); // update hitbox sizes
    int32_t getId(); //returns the id of the entity
    void setId(int32_t num);//Sets the ID, ONLY USE SPARINGLY



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
