#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H
#include<SDL2/SDL.h>
#include "Marine.h"
#include "CollisionHandler.h"
#include <map>
#include <unordered_map>
#include <vector>

class GameManager {
public:
	
	GameManager();
	~GameManager();
	
	void renderObjects(SDL_Renderer* gRenderer, float camX, float camY);
	
	unsigned int createMarine();
	void deleteMarine(unsigned int id);
	bool addMarine(unsigned int id, Marine* newMarine);
	Marine* getMarine(unsigned int id);
	
	void updateCollider(CollisionHandler* ch);
	
private:
	
	std::map<unsigned int, Marine*> marineManager;
	
};


#endif
