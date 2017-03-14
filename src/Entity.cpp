#include "Entity.h"
#include <atomic>

Entity::Entity(int32_t nid, const SDL_Rect &spriteSize):id(nid), spriteClips[0](spriteSize),
        movementHitBox(std::make_shared<HitBox>(x, y, spriteSize)), projectileHitBox(std::make_shared<HitBox>(x, y, spriteSize)),
        damageHitBox(std::make_shared<HitBox>(x, y, spriteSize)), pickupHitBox(std::make_shared<HitBox>(x, y, spriteSize)){
    /*spriteClips[0].x = 0;
    spriteClips[0].y = 0;
    spriteClips[0].w = 100;
    spriteClips[0].h = 100;*/
}

Entity::Entity(int32_t nid, const SDL_Rect &spriteSize, const SDL_Rect &movementSize): id(nid),
        spriteClips[0](spriteSize), movementHitBox(std::make_shared<HitBox>(x, y, movementSize)),
        projectileHitBox(std::make_shared<HitBox>(x, y, spriteSize)), damageHitBox(std::make_shared<HitBox>(x, y, spriteSize)), pickupHitBox(std::make_shared<HitBox>(x, y, spriteSize)){

}


//Weapon drops
Entity::Entity(int32_t nid, const SDL_Rect &spriteSize, const SDL_Rect &movementSize, const SDL_Rect &pickupSize):
        id(nid), spriteClips[0](spriteSize), movementHitBox(std::make_shared<HitBox>(x, y, movementSize)),
        projectileHitBox(std::make_shared<HitBox>(x, y, spriteSize)), damageHitBox(std::make_shared<HitBox>(x, y, spriteSize)), pickupHitBox(std::make_shared<HitBox>(x, y, pickupSize)){

}

Entity(int32_t nid, const SDL_Rect &spriteSize, const SDL_Rect &movementSize, const SDL_Rect &projectileSize,
        const SDL_Rect &damageSize): id(nid), spriteClips[0](spriteSize),
        movementHitBox(std::make_shared<HitBox>(x, y, movementSize)),
        projectileHitBox(std::make_shared<HitBox>(x, y, projectileSize)),
        damageHitBox(std::make_shared<HitBox>(x, y, damageSize)),
        pickupHitBox(std::make_shared<HitBox>(x, y, spriteSize)){

}


//movables and marines
Entity(int32_t nid, const SDL_Rect &spriteSize, const SDL_Rect &movementSize, const SDL_Rect &projectileSize,
        const SDL_Rect &damageSize, const SDL_Rect &pickupSize): id(nid), spriteClips[0](spriteSize), movementHitBox(std::make_shared<HitBox>(x, y, movementSize)),
        projectileHitBox(std::make_shared<HitBox>(x, y, projectileSize)),
        damageHitBox(std::make_shared<HitBox>(x, y, damageSize)),
        pickupHitBox(std::make_shared<HitBox>(x, y, pickupSize)){

}

Entity::Entity(const Entity &e): spriteClips(e.spriteClips),movementHitBox(e.movementHitBox),
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

int32_t generateID() {
    static std::atomic<int32_t> counter{-1};
    return ++counter;
}
