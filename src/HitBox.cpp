 #include "HitBox.h"


HitBox::HitBox() {

}

HitBox::~HitBox() {

}

const SDL_Rect& HitBox::getRect() const{
	return this->rect;
}

void HitBox::move(float x, float y){
	this->rect.x = x;
	this->rect.y = y;
}


void HitBox::setRect(const SDL_Rect rerect) {
	this->rect = rerect;
}

void HitBox::setFriendly(bool isFriendly) {
	this->playerFriendly = 	isFriendly;
}

bool HitBox::isPlayerFriendly() {
	return 	this->playerFriendly;
}
