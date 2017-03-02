#include "Entity.h"

Entity::Entity() {
    spriteClips[0].x = 0;
    spriteClips[0].y = 0;
    spriteClips[0].w = 100;
    spriteClips[0].h = 100;
    movementHitBox.setRect(spriteClips[0]);
    movementHitBox.move(getX(), getY());
    movementHitBox.attached = this;
    projectileHitBox.setRect(spriteClips[0]);
    projectileHitBox.move(getX(), getY());
    projectileHitBox.attached = this;
    damageHitBox.setRect(spriteClips[0]);
    damageHitBox.move(getX(), getY());
    damageHitBox.attached = this;
}

Entity::~Entity() {
    texture.free();
}

// Set x coordinate
void Entity::setX(float px) {
    x = px;
}

// Set y coordinate
void Entity::setY(float py) {
    y = py;
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
void Entity::setPosition(float x, float y) {
    //Move the Entity left or right
    setX(x);

    //Move the Entity up or down
    setY(y);

}
