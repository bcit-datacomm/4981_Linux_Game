#ifndef ENTITY_H
#define ENTITY_H

#include <string>
#include <memory>
#include <SDL2/SDL.h>

#include "../collision/HitBox.h"


class Entity {
public:
    //all values the same
    Entity(int32_t nid, const SDL_Rect dest);
    //size and movement different the rest are sprite size
    Entity(int32_t nid, const SDL_Rect dest, const SDL_Rect &movementSize);
    //size movement, and pickup different size, the rest are sprite size
    Entity(int32_t nid, const SDL_Rect dest, const SDL_Rect &movementSize,
        const SDL_Rect &pickupSize);
    //pickup is the same as sprite size
    Entity(int32_t nid, const SDL_Rect dest, const SDL_Rect &movementSize,
        const SDL_Rect &projectileSize, const SDL_Rect &damageSize);
    //all values are different
    Entity(int32_t nid, const SDL_Rect dest, const SDL_Rect &movementSize,
        const SDL_Rect &projectileSize, const SDL_Rect &damageSize, const SDL_Rect &pickupSize);

    ~Entity() = default;


    Entity(const Entity &e);
    virtual void onCollision();
    virtual void collidingProjectile(const int damage);
    void setPosition(const float x, const float y); // Set marine position
    void setX(float px); //set x coordinate
    void setY(float py); //set y coordinate
    float getX() const; // get x coordinate
    float getY() const; // get y coordinate
    int getW() const;// get w of dest rect
    int getH() const;// get h of dest rect
    void updateHitBoxes(); // update hitbox positions
    void updateRectHitBoxes(); // update hitbox sizes

    int32_t getId()const{return id;}; //returns the id of the entity


    const HitBox& getMoveHitBox()const {return movementHitBox;};
    const HitBox& getProHitBox()const {return projectileHitBox;};
    const HitBox& getDamHitBox()const {return damageHitBox;};
    const HitBox& getPickUpHitBox()const {return pickupHitBox;};

    const SDL_Rect getRelativeDestRect(const SDL_Rect& view) const;

    const SDL_Rect& getDestRect() const {return destRect;};
    const SDL_Rect& getSrcRect() const {return srcRect;};
    void setDestRect(int x, int y, int width, int height);
    void setSrcRect(int x, int y, int width, int height);

    void moveMoveHitBox(int x, int y) { movementHitBox.move(x,y);};
    void moveProHitBox(int x, int y) { projectileHitBox.move(x,y);};
    void moveDamHitBox(int x, int y) { damageHitBox.move(x,y);};
    void movePickUpHitBox(int x, int y) { pickupHitBox.move(x,y);};

private:

    int32_t id; //is the index num of the entity in its respective manager
    SDL_Rect destRect;
    SDL_Rect srcRect;
    HitBox movementHitBox;
    HitBox projectileHitBox;
    HitBox damageHitBox;
    HitBox pickupHitBox;
    float x;
    float y;
};

#endif
