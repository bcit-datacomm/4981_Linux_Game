#include "SpriteManager.h"

SpriteManager::SpriteManager() {
	//create 2d map
		//1: string, naming sprite sheet
		//2: png, naming sprite sheet 

	loadSpriteCollection();
//	spriteCollection.insert(std::pair<int, LTexture>(1, "assets/texture/mohawk_sprite_sheet_0.25.png"));
}

SpriteManager::~SpriteManager() {
	//free map
}

void SpriteManager::loadSpriteCollection() {
	//load map floor textures
	//load map objects
	//load characters (zombies + players)
	//load weapons
	//load turret

	
//"assets/texture/mohawk_sprite_sheet_0.25.png"
	SDL_Surface* loadedSurface = IMG_Load( "assets/texture/mohawk_sprite_sheet_0.25.png");
	spriteCollection.insert(std::pair<int, SDL_Surface* >(1, loadedSurface));	
}

//get sprite collection
