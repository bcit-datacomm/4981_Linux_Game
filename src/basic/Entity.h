/*------------------------------------------------------------------------------
* Header: Entity.h
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
#ifndef ENTITY_H
#define ENTITY_H

#include <string>
#include <memory>
#include <SDL2/SDL.h>

#include "../collision/HitBox.h"


class Entity {
public:
    //all values the same
    Entity(const int32_t nid, const SDL_Rect& dest);
    //size and movement different the rest are sprite size
    Entity(const int32_t nid, const SDL_Rect& dest, const SDL_Rect& movementSize);
    //size movement, and pickup different size, the rest are sprite size
    Entity(const int32_t nid, const SDL_Rect& dest, const SDL_Rect& movementSize,
        const SDL_Rect& pickupSize);
    //pickup is the same as sprite size
    Entity(const int32_t nid, const SDL_Rect& dest, const SDL_Rect& movementSize,
        const SDL_Rect& projectileSize, const SDL_Rect& damageSize);
    //all values are different
    Entity(const int32_t nid, const SDL_Rect& dest, const SDL_Rect& movementSize,
        const SDL_Rect& projectileSize, const SDL_Rect& damageSize, const SDL_Rect& pickupSize);

    ~Entity() = default;


    Entity(const Entity& e);
    virtual void onCollision();
    virtual void collidingProjectile(const int damage);
    void setPosition(const float x, const float y); // Set marine position
    void setX(float px); //set x coordinate
    void setY(float py); //set y coordinate
    float getX() const {return x;} // Get x coordinate
    float getY() const{return y;} // Get y coordinate
    int getW() const{return destRect.w;} // Get width
    int getH() const{return destRect.h;} // Get height
    void setW(const int w) {destRect.w = w;} // Set width
    void setH(const int h) {destRect.h = h;} // Set height
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
    void setDestRect(const int x, const int y, const int width, const int height);
    void setSrcRect(const int x, const int y, const int width, const int height);

    void moveMoveHitBox(const int x, const int y) { movementHitBox.move(x,y);};
    void moveProHitBox(const int x, const int y) { projectileHitBox.move(x,y);};
    void moveDamHitBox(const int x, const int y) { damageHitBox.move(x,y);};
    void movePickUpHitBox(const int x, const int y) { pickupHitBox.move(x,y);};

    const std::pair<float, float> getMoveCoord() const {return {movementHitBox.getRect().x, movementHitBox.getRect().y};};
    const std::pair<float, float> getDestCoord() const {return {destRect.x, destRect.y};};
    const std::pair<float, float> getSrcCoord() const {return {srcRect.x, srcRect.y};};
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
