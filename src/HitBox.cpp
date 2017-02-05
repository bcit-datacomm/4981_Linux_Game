 #include "HitBox.h"


HitBox::HitBox() {
	
}

HitBox::~HitBox() {
	
}
	
bool HitBox::collisionCheck(HitBox& nearHitBox) {
	if (SDL_HasIntersection(&this->getRect(), &nearHitBox.getRect())) {
		printf("HIT\n");
		return true;
	} else {
		printf("GOOD\n");
		return false;	
	}
}

const SDL_Rect& HitBox::getRect() {
	return this->rect;	
}

void HitBox::move(float x, float y) {
	this->rect.x = x;
	this->rect.y = y;
}


void HitBox::setRect(const SDL_Rect rerect) {
	this->rect = rerect;
}
