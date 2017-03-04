#include "Barricade.h"
#include "GameManager.h"
#include <math.h>
#include <random>
#define PI 3.14159265

Barricade::Barricade(int health, int state, bool boolPlaceable, bool boolPlaced) 
                    : Object(BARRICADE_HEIGHT, BARRICADE_WIDTH),
                      health(health), state(state), 
                      boolPlaceable(boolPlaceable), boolPlaced(boolPlaced) {
	printf("Create Barricade\n");
}

Barricade::~Barricade() {
	printf("Destory Barricade\n");
}

bool Barricade::checkPlaceablePosition(float playerX, float playerY, float moveX, float moveY, CollisionHandler  &ch){
	float distanceX = (playerX - moveX) * (playerX - moveX);
	float distanceY = (playerY - moveY) * (playerY - moveY);
	float distance = sqrt(abs(distanceX+distanceY));
	if(distance>200){
		boolPlaceable = false;
	}else
		boolPlaceable = true;

	if(boolPlaceable){
        if(ch.detectMovementCollision(movementHitBox.get()))
			boolPlaceable = false;
 
	}
	return boolPlaceable;
}

bool Barricade::isPlaceable(){
	return boolPlaceable;
}

bool Barricade::isPlaced(){
    return boolPlaced;
}

// Move Zombie by x and y amount
void Barricade::move(float playerX, float playerY, float moveX, float moveY, CollisionHandler &ch) {
	setPosition(moveX, moveY);
    //`setX(100);

	if(this->checkPlaceablePosition(playerX, playerY, moveX, moveY, ch))
		texture.setAlpha(200);
	else
		texture.setAlpha(30);
}


void Barricade::onCollision() {
	// Do nothing for now
}

void Barricade::collidingProjectile(int damage) {
	health -= damage;
}

void Barricade::placeBarricade(){
    texture.setAlpha(255);
    boolPlaced=true;
}
