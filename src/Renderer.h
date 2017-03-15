#ifndef RENDERER_H
#define RENDERER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <array>
#include <string>

#include "SpriteTypes.h"

/* DEVELOPER: Michael Goll
** DESIGNER:  Michael Goll
** DATE:      March 14, 2017
*/

//-------- Game Screens --------
#define MAIN_SCREEN "assets/texture/Map/"
//#define LOBBY_SCREEN "assets/texture/Map/" //<-- Will be used at a later date to show lobby

//-------- Map Textures --------
#define MAP_TEXTURE_PATH "assets/texture/Map/"

//terraformed terrain
#define TEXTURE_DIRT   MAP_TEXTURE_PATH "dirt_grass.png"
//barren dirt
#define TEXTURE_BARREN MAP_TEXTURE_PATH "mars_dirt.png"
//dead grass
#define TEXTURE_MIDDLE MAP_TEXTURE_PATH "middle_dirt.png"

//size of the texture
#define TEXTURE_SIZE 250

//size of the marine
#define MARINE_SIZE 100

//TODO: remove these textures, temporary for now
#define TEMP_MARINE_TEXTURE "assets/texture/arrow.png"
#define TEMP_ZOMBIE_TEXTURE "assets/texture/babyz1.png"

//Sprite Sheet folder path
#define SPRITE_PATH "assets/texture/SpriteSheets/"

//-------- Map Objects Sprite Sheet --------
#define MAP_OBJECTS SPRITE_PATH "mapObjects.png"

//-------- Zombie Sprite Sheets --------
#define ZOMBIE_BABYZ  SPRITE_PATH "babyz.png"
#define ZOMBIE_DIGGER SPRITE_PATH "digger.png"
#define ZOMBIE_BOSS   SPRITE_PATH "zombieboss.png"

//-------- Marine Sprite Sheet --------
#define PLAYER_MOHAWK  SPRITE_PATH "mohawk.png"

//-------- Weapons Sprite Sheet --------
#define WEAPONS SPRITE_PATH "weapons.png"
#define LASER   SPRITE_PATH "laser.png"
//#define TURRETS        SPRITE_PATH ""

#define TOTAL_SPRITES 10 //number of total sprites

//temporary sprite, will be removed later
#define REPLACE_ME "assets/texture/replace_me.png"


class Renderer {
public:
    //returns the instance if it exists, otherwise creates one
    static Renderer * instance();
    ~Renderer();

    //returns the sprite or sprite sheet that the object is looking to render
    static SDL_Texture * getTexture(int spriteType) {return _sprites.at(spriteType);};
    //gets the renderer
    static SDL_Renderer * getRenderer() {return _renderer;};
    //sets the window
    static void setWindow(SDL_Window * window);
    //loads all the sprites specified in Renderer.h
    static void loadSprites();
    //renders all of the sprites within the camera viewport
    static void render(int x, int y, int w, int h, TEXTURES spriteType, SDL_Rect* clip = nullptr,
                       double angle = 0.0, SDL_Point* center = nullptr, SDL_RendererFlip flip = SDL_FLIP_NONE);

private:
    Renderer(){}

    static Renderer * rInstance;
    static SDL_Renderer * _renderer;
    static int _index;
    static SDL_Window * _window;

    //array of all sprites in the game
    static std::array<SDL_Texture *, TOTAL_SPRITES> _sprites;
    //creates a texture from a file
    static void createTexture(std::string filePath);
    //sets the renderer
    static void setRenderer();
};

#endif
