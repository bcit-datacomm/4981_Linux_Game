#include "Entity.h"
#include <atomic>

Entity::Entity():id(generateID()) {
    spriteClips[0].x = 0;
    spriteClips[0].y = 0;
    spriteClips[0].w = 100;
    spriteClips[0].h = 100;
    movementHitBox = std::make_shared<HitBox>(x, y, spriteClips[0]);
    projectileHitBox = std::make_shared<HitBox>(x, y, spriteClips[0]);
    damageHitBox = std::make_shared<HitBox>(x, y, spriteClips[0]);
    pickupHitBox = std::make_shared<HitBox>(x, y, spriteClips[0]);

}

Entity::Entity(const Entity &e): spriteClips(e.spriteClips),movementHitBox(e.movementHitBox),
        projectileHitBox(e.projectileHitBox),damageHitBox(e.damageHitBox), pickupHitBox(e.pickupHitBox),
        id(e.getId()){
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

void Entity::collidingProjectile(const int damage){
    //do nothing
}

int32_t generateID() {
    static std::atomic<int32_t> counter{-1};
    return ++counter;
}
