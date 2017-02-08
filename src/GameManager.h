#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H
#include<SDL2/SDL.h>
#include "Marine.h"
#include "CollisionHandler.h"
#include "Object.h"
#include "Zombie.h"
#include <map>
#include <unordered_map>
#include <vector>

class GameManager {
public:
	
	GameManager();
	~GameManager();
	
	void renderObjects(SDL_Renderer* gRenderer, float camX, float camY); // Render all objects in level
	
	// Methods for creating, getting, and deleting marines from the level.
	unsigned int createMarine();
	void deleteMarine(unsigned int id);
	bool addMarine(unsigned int id, Marine* newMarine);
	Marine* getMarine(unsigned int id);
	
	void updateCollider(); // Updates CollisionHanlder
	void updateMarines(const float& delta); // Update marine actions
	void updateZombies(const float& delta); // Update zombie actions
	
	unsigned int addObject(Object*);
	unsigned int addZombie(Zombie*);
private:
	
	CollisionHandler* collisionHandler = NULL;
	std::map<unsigned int, Marine*> marineManager;
	std::map<unsigned int, Object*> objectManager;	
	std::map<unsigned int, Zombie*> zombieManager;	
};


#endif
