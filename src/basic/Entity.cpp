#include "Entity.h"
#include <atomic>
#include <cstdint>
#include "../log/log.h"

Entity::Entity(int32_t nid, const SDL_Rect dest):id(nid), destRect(dest), srcRect({0,0, dest.w, dest.h}),
        movementHitBox(dest), projectileHitBox(dest), damageHitBox(dest), pickupHitBox(dest), x(dest.x),
        y(dest.y) {
}

Entity::Entity(int32_t nid, const SDL_Rect dest, const SDL_Rect &movementSize): id(nid), destRect(dest),
        srcRect({0,0, dest.w, dest.h}), movementHitBox(movementSize), projectileHitBox(dest),
        damageHitBox(dest), pickupHitBox(dest), x(dest.x), y(dest.y) {

}

//Weapon drops
Entity::Entity(int32_t nid, const SDL_Rect dest, const SDL_Rect &movementSize,
        const SDL_Rect &pickupSize): id(nid), destRect(dest), srcRect({0,0, dest.w, dest.h}),
        movementHitBox(movementSize), projectileHitBox(dest), damageHitBox(dest),pickupHitBox(pickupSize),
        x(dest.x), y(dest.y) {

}

Entity::Entity(int32_t nid, const SDL_Rect dest, const SDL_Rect &movementSize, const SDL_Rect &projectileSize,
        const SDL_Rect &damageSize): id(nid), destRect(dest), srcRect({0,0, dest.w, dest.h}),
        movementHitBox(movementSize), projectileHitBox(projectileSize), damageHitBox(damageSize),
        pickupHitBox(dest), x(dest.x), y(dest.y) {
}


//movables and marines
Entity::Entity(int32_t nid,  const SDL_Rect dest, const SDL_Rect &movementSize, const SDL_Rect &projectileSize,
        const SDL_Rect &damageSize, const SDL_Rect &pickupSize): id(nid), destRect(dest),
        srcRect({0,0, dest.w, dest.h}), movementHitBox(movementSize), projectileHitBox(projectileSize),
        damageHitBox(damageSize), pickupHitBox(pickupSize), x(dest.x), y(dest.y) {

}

Entity::Entity(const Entity &e): id(e.id), destRect(e.destRect), srcRect(e.srcRect),
        movementHitBox(e.movementHitBox), projectileHitBox(e.projectileHitBox),
        damageHitBox(e.damageHitBox), pickupHitBox(e.pickupHitBox), x(e.x), y(e.y) {
}

const SDL_Rect Entity::getRelativeDestRect(const SDL_Rect& view) const {
    return {destRect.x - view.x , destRect.y - view.y, static_cast<int>(destRect.w), static_cast<int>(destRect.h)};
}

// Set x coordinate
void Entity::setX(const float px) {
    x = px;
    destRect.x = px;
    updateHitBoxes();
}

// Set y coordinate
void Entity::setY(const float py) {
    y = py;
    destRect.y = py;
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

// Get x coordinate
int Entity::getW() const{
    return destRect.w;
}

// Get y coordinate
int Entity::getH() const{
    return destRect.h;
}
// Set Entity by x and y amount
void Entity::setPosition(float px, float py) {
    x = px;
    y = py;
    updateHitBoxes();
}

//updates the hit boxes
void Entity::updateHitBoxes() {
    movementHitBox.move(x, y);
    projectileHitBox.move(x, y);
    damageHitBox.move(x, y);
    pickupHitBox.move(x - 10, y - 10);
}

//updates the rectangle for the hitbox
void Entity::updateRectHitBoxes() {
    movementHitBox.setRect(destRect);
    projectileHitBox.setRect(destRect);
    damageHitBox.setRect(destRect);
    pickupHitBox.setRect(destRect);
}

void Entity::onCollision() {
    //do nothing
}

void Entity::collidingProjectile(const int damage) {
    //do nothing
}

//sets the destination rect of where to render stuff
void Entity::setDestRect(const int x, const int y, const int width, const int height) {
    destRect.x = x;
    destRect.y = y;
    destRect.w = width;
    destRect.h = height;
}

//sets the source rect, used for sprites and textures
void Entity::setSrcRect(const int x, const int y, const int width, const int height) {
    srcRect.x = x;
    srcRect.y = y;
    srcRect.w = width;
    srcRect.h = height;
}
