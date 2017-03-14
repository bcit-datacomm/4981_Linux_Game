#include "Barricade.h"
#include "../game/GameManager.h"
#include <math.h>
#include <random>
#define PI 3.14159265

Barricade::Barricade(int health, int state, bool placeable, bool placed)
        : Object(BARRICADE_HEIGHT, BARRICADE_WIDTH),health(health), state(state), placeable(placeable),
        placed(placed) {
    printf("Create Barricade\n");
}

Barricade::~Barricade() {
    printf("Destory Barricade\n");
}

bool Barricade::checkPlaceablePosition(const float playerX, const float playerY,
        const float moveX, const float moveY, CollisionHandler  &ch) {
    const float distanceX = (playerX - moveX) * (playerX - moveX);
    const float distanceY = (playerY - moveY) * (playerY - moveY);
    const float distance = sqrt(abs(distanceX+distanceY));

    placeable = (distance <= 200);

    SDL_Rect checkBox;
    checkBox.h = 100;
    checkBox.w = 100;
    checkBox.x = getX();
    checkBox.y = getY();
    HitBox hitBox(getX(), getY(), checkBox);

    if(placeable && ch.detectMovementCollision(this)){
        placeable = false;
    }


    return placeable;
}

bool Barricade::isPlaceable(){
    return placeable;
}

bool Barricade::isPlaced(){
    return placed;
}

// Move Zombie by x and y amount
void Barricade::move(const float playerX, const float playerY, const float moveX,
        const float moveY, CollisionHandler &ch) {
    setPosition(moveX, moveY);
    //`setX(100);

    if(checkPlaceablePosition(playerX, playerY, moveX, moveY, ch)) {
        texture.setAlpha(200);
    } else {
        texture.setAlpha(30);
    }
}


void Barricade::onCollision() {
    // Do nothing for now
}

void Barricade::collidingProjectile(const int damage) {
    health -= damage;
}

void Barricade::placeBarricade(){
    texture.setAlpha(255);
    placed=true;
}
