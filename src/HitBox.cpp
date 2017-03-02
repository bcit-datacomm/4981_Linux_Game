 #include "HitBox.h"


HitBox::HitBox() {

}

HitBox::~HitBox() {

}

const SDL_Rect& HitBox::getRect() const{
    return rect;
}

void HitBox::move(float x, float y){
    rect.x = x;
    rect.y = y;
}


void HitBox::setRect(const SDL_Rect rerect) {
    rect = rerect;
}

void HitBox::setFriendly(bool isFriendly) {
    playerFriendly =     isFriendly;
}

bool HitBox::isPlayerFriendly() {
    return     playerFriendly;
}
