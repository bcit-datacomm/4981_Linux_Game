 #include "HitBox.h"


HitBox::HitBox(float x, float y, SDL_Rect rect, Entity* attached) : attached(attached) {
	setRect(rect);
	move(x,y);
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

bool HitBox::isPlayerFriendly() const {
    return     playerFriendly;
}
