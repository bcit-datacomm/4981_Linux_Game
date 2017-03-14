#include "Entity.h"
#include <atomic>

Entity::Entity(int32_t nid, const SDL_Rect &spriteSize):id(nid), movementHitBox(sspriteSize),
        projectileHitBox(spriteSize), damageHitBox(spriteSize), pickupHitBox(spriteSize){
}

Entity::Entity(int32_t nid, const SDL_Rect &spriteSize, const SDL_Rect &movementSize): id(nid),
        movementHitBox(movementSize), projectileHitBox(spriteSize), damageHitBox(spriteSize),
        pickupHitBox(spriteSize){

}

//Weapon drops
Entity::Entity(int32_t nid, const SDL_Rect &spriteSize, const SDL_Rect &movementSize,
        const SDL_Rect &pickupSize): id(nid), movementHitBox(movementSize), projectileHitBox(spriteSize),
        damageHitBox(spriteSize),pickupHitBox(pickupSize){

}

Entity(int32_t nid, const SDL_Rect &spriteSize, const SDL_Rect &movementSize, const SDL_Rect &projectileSize,
        const SDL_Rect &damageSize): id(nid), movementHitBox(movementSize), projectileHitBox(projectileSize),
        damageHitBox(damageSize), pickupHitBox(spriteSize){

}


//movables and marines
Entity(int32_t nid, const SDL_Rect &spriteSize, const SDL_Rect &movementSize, const SDL_Rect &projectileSize,const SDL_Rect &damageSize, const SDL_Rect &pickupSize): id(nid), movementHitBox(movementSize), projectileHitBox(projectileSize), damageHitBox(damageSize), pickupHitBox(pickupSize){

}

Entity::Entity(const Entity &e): spriteClips(e.spriteClips), movementHitBox(e.movementHitBox),
        projectileHitBox(e.projectileHitBox),damageHitBox(e.damageHitBox), pickupHitBox(e.pickupHitBox),
        id(e.id){
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

void Entity::updateRectHitBoxes() {
    movementHitBox->setRect(spriteClips[0]);
    projectileHitBox->setRect(spriteClips[0]);
    damageHitBox->setRect(spriteClips[0]);
    pickupHitBox->setRect(spriteClips[0]);
}

void Entity::onCollision(){
    //do nothing
}

void Entity::collidingProjectile(int damage){
    //do nothing
}


int32_t Entity::getId(){
    return id;
}

void Entity::setId(int32_t num){
    id = num;
}
