#ifndef SPRITEMANAGER_H
#define SPRITEMANAGER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>
#include <map>

//define all the sprite sheet file paths here

class SpriteManager{
public:
	SpriteManager();
	~SpriteManager();

	//getSpriteCollection();
	
private:
	std::map<int, SDL_Surface*> spriteCollection; 	
	void loadSpriteCollection();
};

#endif