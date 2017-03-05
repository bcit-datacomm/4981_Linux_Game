#ifndef HITBOX_H
#define HITBOX_H
#include <SDL2/SDL.h>

class Entity;

class HitBox {
public:

    HitBox(float x, float y, SDL_Rect rect, Entity* attached);
    ~HitBox();

    void move(float x, float y);
    void setRect(const SDL_Rect rect);
    const SDL_Rect& getRect() const;

    void setFriendly(bool isFriendly);
    bool isPlayerFriendly() const;

    Entity* attached; // Entity that it is linked to

private:

    SDL_Rect rect;
    bool playerFriendly = false;

};


#endif
