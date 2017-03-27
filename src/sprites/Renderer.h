#ifndef RENDERER_H
#define RENDERER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <array>
#include <string>
#include <map>

#include "../sprites/SpriteTypes.h"
#include "../log/log.h"

/*
 * DEVELOPER: Michael Goll
 * DESIGNER:  Michael Goll
 * DATE:      March 14, 2017
 */

//-------- Game Screens --------
const std::string MAIN_SCREEN = "assets/TitleScreen_Marz.png";

const std::string TEXTBOX_TEXTURE = "assets/texture/textbox.png";
//#define LOBBY_SCREEN "assets/texture/Map/" // <-- Will be used at a later date to show lobby bg

//-------- Map Textures --------
const std::string MAP_TEXTURE_PATH = "assets/texture/Map/";
const std::string TEXTURE_DIRT = MAP_TEXTURE_PATH + "dirt_grass.png"; //terraformed terrain
const std::string TEXTURE_BARREN = MAP_TEXTURE_PATH + "mars_dirt.png"; //barren dirt
const std::string TEXTURE_MIDDLE = MAP_TEXTURE_PATH + "middle_dirt.png"; //dead grass

//TODO: remove these textures, temporary for now
const std::string TEMP_MARINE_TEXTURE = "assets/texture/arrow.png";
const std::string TEMP_ZOMBIE_TEXTURE = "assets/texture/babyz1.png";

//Sprite Sheet folder path
const std::string SPRITE_PATH = "assets/texture/SpriteSheets/";

//-------- Map Objects Sprite Sheet --------
const std::string MAP_OBJECTS = SPRITE_PATH + "mapObjects.png";

//-------- Zombie Sprite Sheets --------
const std::string ZOMBIE_BABYZ = SPRITE_PATH + "babyz.png";
const std::string ZOMBIE_DIGGER = SPRITE_PATH + "digger.png";
const std::string ZOMBIE_BOSS = SPRITE_PATH + "zombieboss.png";

//-------- Marine Sprite Sheet --------
const std::string PLAYER_MOHAWK = SPRITE_PATH + "mohawk.png";

//-------- Weapons Sprite Sheet --------
const std::string WEAPONS = SPRITE_PATH + "weapons.png";
const std::string LASER = SPRITE_PATH  + "laser.png";

const std::string REPLACE_ME = "assets/texture/replace_me.png"; //temporary sprite, will be removed later

static constexpr int TEXTURE_SIZE = 250; //size of the texture
static constexpr int MARINE_SIZE = 100; //size of the marine
static constexpr int TOTAL_SPRITES = 20; //number of total sprites


class Renderer {
public:


        /**
         * DEVELOPER: Michael Goll
         * DESIGNER:  Michael Goll
         * DATE:      March 14, 2017
         *
         * REVISED: Isaac Morneau, March 25, 2017
         *      changed pointers and static to references
         *
         * returns the instance for rendering
         */
        static Renderer& instance() {
            return sInstance;
        }


        //returns the sprite or sprite sheet that the object is looking to render
        SDL_Texture *getTexture(int spriteType);

        /**
         * DEVELOPER: Michael Goll
         * DESIGNER: Michael Goll
         * DATE: March 14, 2017
         * gets the rederer directly
         */
        SDL_Renderer *getRenderer() {
            return renderer;
        };

        //sets the window
        void setWindow(SDL_Window *win);

        //loads all the sprites specified in Renderer.h
        void loadSprites();

        TTF_Font *loadFont(const std::string& fonts, const int size);

        //creates a texture from a font file
        void createText(const TEXTURES index, TTF_Font *font, const std::string& text, const SDL_Color& colour);

        int createTempText(TTF_Font *font, const std::string& text, const SDL_Color& colour);

        //creates a temporary texture
        int createTempTexture(const std::string& filePath);

        //renders all of the sprites within the camera viewport
        void render(const SDL_Rect& dest, const TEXTURES spriteType, double angle = 0.0,
                const SDL_Point *center = nullptr, const SDL_RendererFlip flip = SDL_FLIP_NONE);
        void render(const SDL_Rect& dest, const int spriteType, double angle = 0.0,
                const SDL_Point *center = nullptr, const SDL_RendererFlip flip = SDL_FLIP_NONE);

        //renders all of the sprites within the camera viewport
        void render(const SDL_Rect& dest, const TEXTURES spriteType, const SDL_Rect& clip, double angle = 0.0,
                const SDL_Point *center = nullptr, const SDL_RendererFlip flip = SDL_FLIP_NONE);
        void render(const SDL_Rect& dest, const int spriteType, const SDL_Rect& clip, double angle = 0.0,
                const SDL_Point *center = nullptr, const SDL_RendererFlip flip = SDL_FLIP_NONE);


private:
        Renderer(): tempIndex(1000) {}
        ~Renderer();

        static Renderer sInstance;
        SDL_Renderer* renderer;
        SDL_Window* window;
        int tempIndex;

        //array of all sprites in the game
        std::map<int, SDL_Texture*> sprites;

        //creates a texture from a file
        void createTexture(const TEXTURES index, const std::string& filePath);
        void createTexture(const int index, const std::string& filePath);

        //sets the renderer
        void setRenderer();
};

#endif
