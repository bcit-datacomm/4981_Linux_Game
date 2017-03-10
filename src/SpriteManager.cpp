#include "SpriteManager.h"

SpriteManager::SpriteManager() {
   loadFiles();
}

SpriteManager::~SpriteManager() {
    for(auto const& surface : _sprites) {
        SDL_FreeSurface(surface.second);
    }
}

//load all sprites sheets
void SpriteManager::loadFiles() {
 loadSprite(MAP_OBJECTS);
 loadSprite(ZOMBIE_BABYZ);
 loadSprite(ZOMBIE_DIGGER);
 loadSprite(ZOMBIE_BOSS);
 loadSprite(PLAYER_MOHAWK);
 loadSprite(WEAPONS);
 loadSprite(LASER);
}

//Adds a texture to the _sprites map
void SpriteManager::loadSprite(const char * fileName) {

    SDL_Surface * sprite = NULL;
    sprite = IMG_Load(fileName);
    if (sprite == NULL ) {
        printf( "Failed to load sprite: %s\n", fileName);
    }

    _sprites[fileName] = sprite;
}

//Gets a specific sprite sheet
SDL_Surface* SpriteManager::getSurface(const char * fileName) {
    
    SDL_Surface * sprite = _sprites[fileName];    

    printf("Loading: %s\t- %s\n", fileName, SDL_GetError());

    return sprite;
}