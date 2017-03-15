#include "Entity.h"
#include <atomic>
#include <cstdint>

Entity::Entity(int32_t nid, const SDL_Rect dest):id(nid), destRect(dest), srcRect({0,0, dest.w, dest.h}), movementHitBox(dest),
        projectileHitBox(dest), damageHitBox(dest), pickupHitBox(dest){
}

Entity::Entity(int32_t nid, const SDL_Rect dest, const SDL_Rect &movementSize): id(nid), destRect(dest), srcRect({0,0, dest.w, dest.h}),
        movementHitBox(movementSize), projectileHitBox(dest), damageHitBox(dest),
        pickupHitBox(dest){

}

//Weapon drops
Entity::Entity(int32_t nid, const SDL_Rect dest, const SDL_Rect &movementSize,
        const SDL_Rect &pickupSize): id(nid), destRect(dest), srcRect({0,0, dest.w, dest.h}), movementHitBox(movementSize), projectileHitBox(dest),
        damageHitBox(dest),pickupHitBox(pickupSize){

}

Entity::Entity(int32_t nid, const SDL_Rect dest, const SDL_Rect &movementSize, const SDL_Rect &projectileSize,
        const SDL_Rect &damageSize): id(nid), destRect(dest), srcRect({0,0, dest.w, dest.h}), movementHitBox(movementSize), projectileHitBox(projectileSize),
        damageHitBox(damageSize), pickupHitBox(dest){

}


//movables and marines
Entity::Entity(int32_t nid,  const SDL_Rect dest, const SDL_Rect &movementSize, const SDL_Rect &projectileSize,const SDL_Rect &damageSize, const SDL_Rect &pickupSize): id(nid), destRect(dest), srcRect({0,0, dest.w, dest.h}), movementHitBox(movementSize), projectileHitBox(projectileSize), damageHitBox(damageSize), pickupHitBox(pickupSize){

}

Entity::Entity(const Entity &e): id(e.id), destRect(e.destRect), srcRect(e.srcRect), movementHitBox(e.movementHitBox),
        projectileHitBox(e.projectileHitBox),damageHitBox(e.damageHitBox), pickupHitBox(e.pickupHitBox)
        {
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
    movementHitBox.move(x, y);
    projectileHitBox.move(x, y);
    damageHitBox.move(x, y);
    pickupHitBox.move(x, y);
}

void Entity::updateRectHitBoxes() {
    movementHitBox.setRect(destRect);
    projectileHitBox.setRect(destRect);
    damageHitBox.setRect(destRect);
    pickupHitBox.setRect(destRect);
}

void Entity::onCollision(){
    //do nothing
}

void Entity::collidingProjectile(int damage){
    //do nothing
}

void Entity::setId(int32_t num){
    id = num;
}

void Entity::setDestRect(int x, int y, int width, int height){
    destRect.x = x;
    destRect.y = y;
    destRect.w = width;
    destRect.h = height;
}

void Entity::setSrcRect(int x, int y, int width, int height){
    srcRect.x = x;
    srcRect.y = y;
    srcRect.w = width;
    srcRect.h = height;
}
