#include "Entity.h"

Entity::Entity() {
    spriteClips[0].x = 0;
    spriteClips[0].y = 0;
    spriteClips[0].w = 100;
    spriteClips[0].h = 100;
    movementHitBox = std::make_shared<HitBox>(x, y, spriteClips[0], this);
    projectileHitBox = std::make_shared<HitBox>(x, y, spriteClips[0], this);
    damageHitBox = std::make_shared<HitBox>(x, y, spriteClips[0], this);
    pickupHitBox = std::make_shared<HitBox>(x, y, spriteClips[0], this);
}

Entity::~Entity() {
    texture.free();
}

// Set x coordinate
void Entity::setX(float px) {
    x = px;
    updateHitBoxes();
}

// Set y coordinate
void Entity::setY(float py) {
    y = py;
    updateHitBoxes();
}

// Get x coordinate
float Entity::getX() const{
    return x;
}

// Get y coordinate
float Entity::getY() const{
    return y;
}

// Set Entity by x and y amount
void Entity::setPosition(float px, float py) {
    x = px;
    y = py;
    updateHitBoxes();
}

void Entity::updateHitBoxes() {
	movementHitBox->move(x, y);
	projectileHitBox->move(x, y);
	damageHitBox->move(x, y);
	pickupHitBox->move(x, y);
}
