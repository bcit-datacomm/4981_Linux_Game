#ifndef HITBOX_H
#define HITBOX_H
#include <SDL2/SDL.h>

class HitBox {
public:
    HitBox(const float& x, const float& y, SDL_Rect rect);
    ~HitBox() = default;

    void move(const float& x, const float& y);
    void setRect(const SDL_Rect& rect);
    const SDL_Rect& getRect() const;

    void setFriendly(const bool isFriendly);
    bool isPlayerFriendly() const;


    friend bool operator!=(const HitBox& first, const HitBox& second);

private:
    SDL_Rect rect;
    bool playerFriendly = false;
};

bool operator!=(const HitBox& first, const HitBox& second);
bool operator!=(const SDL_Rect& first, const SDL_Rect& second);

#endif
