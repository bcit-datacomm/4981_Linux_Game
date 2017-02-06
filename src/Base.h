#ifndef BASE_H
#define BASE_H
#include <string>
#include "LTexture.h"
#include "HitBox.h"
#include "Object.h"
#include "CollisionHandler.h"
#include <vector>
#include <map>
#include <utility>
#include <SDL2/SDL.h>

const int BASE_HEIGHT = 100;
const int BASE_WIDTH = 100;

class Base : public Object {
public:
	void onCollision();
	
	void collidingProjectile(int damage);

	std::pair<float, float> addPlayer(unsigned int);

	std::pair<float, float> getSpawnPoints(unsigned int);
	
	Base();
	virtual ~Base();
	
private:
	std::map<unsigned int, std::pair<float, float>> spawnPoints;
	int health = 100;

};
#endif
