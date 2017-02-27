#ifndef BASE_H
#define BASE_H
#include <string>
#include "LTexture.h"
#include "HitBox.h"
#include "Object.h"
#include "CollisionHandler.h"
#include "Player.h"
#include <vector>
#include <map>
#include <utility>
#include <SDL2/SDL.h>

//TODO :  change later.
const int BASE_HEIGHT = 500;
const int BASE_WIDTH = 500;
const int MAP_HEIGHT = 2000;
const int MAP_WIDTH = 2000;

typedef std::pair<float, float> Point;

class Base : public Object {
public:
	void onCollision();
	
	void collidingProjectile(int damage);

	Point getSpawnPoint();
	
	Base();
	virtual ~Base();
	
private:
	int health = 100;
	
};
#endif
