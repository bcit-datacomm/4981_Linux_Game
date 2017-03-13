#ifndef SPRITEMANAGER_H
#define SPRITEMANAGER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>
#include <map>

//Sprite Sheet folder path
#define SPRITE_PATH "assets/texture/SpriteSheets/"

//Sprite Sheets
#define MAP_OBJECTS SPRITE_PATH "mapObjects.png"

//Zombie Sprites Sheets
#define ZOMBIE_BABYZ SPRITE_PATH "babyz.png"
#define ZOMBIE_DIGGER SPRITE_PATH "digger.png"
#define ZOMBIE_BOSS SPRITE_PATH "zombieboss.png"

//Player Sprites Sheets
#define PLAYER_MOHAWK  SPRITE_PATH "mohawk.png"

//Weapons Sprite Sheets
#define WEAPONS SPRITE_PATH "weapons.png"
#define LASER SPRITE_PATH "laser.png"
//#define TURRETS        SPRITE_PATH ""


//maps for storing loaded files.
typedef std::map<std::string, SDL_Surface*> spriteMap;


class SpriteManager {
public:
    SpriteManager();
    ~SpriteManager();

    SDL_Surface* getSurface(const char * fileName);
 
private:
    spriteMap _sprites;

    void loadFiles();
    void loadSprite(const char * fileName);
};

#endif
