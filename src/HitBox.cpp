#include "HitBox.h"


HitBox::HitBox(const float& x, const float& y, SDL_Rect rect){
    setRect(rect);
    move(x,y);
}

const SDL_Rect& HitBox::getRect() const{
    return rect;
}

void HitBox::move(const float& x, const float& y){
    rect.x = x;
    rect.y = y;
}

void HitBox::setRect(const SDL_Rect& rerect) {
    rect = rerect;
}

void HitBox::setFriendly(bool isFriendly) {
    playerFriendly = isFriendly;
}

bool HitBox::isPlayerFriendly() const {
    return playerFriendly;
}

bool operator!=(const HitBox& first, const HitBox& second) {
    return first.rect != second.rect && first.playerFriendly != second.playerFriendly;
}

bool operator!=(const SDL_Rect& first, const SDL_Rect& second) {
    return first.x != second.x && first.y != second.y && first.w != second.h && first.h != second.h;
}
