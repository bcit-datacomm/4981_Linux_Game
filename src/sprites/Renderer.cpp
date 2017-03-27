#include "../sprites/Renderer.h"
#include "../view/Window.h"
#include "../log/log.h"

/**
 * DEVELOPER: Michael Goll
 * DESIGNER:  Michael Goll
 * DATE:      March 14, 2017
 */

Renderer Renderer::sInstance;

/**
 * DEVELOPER: Michael Goll
 * DESIGNER:  Michael Goll
 * DATE:      March 14, 2017
 */
Renderer::~Renderer() {
    for (const auto& s : sprites) {
        if (s.second != nullptr) {
            SDL_DestroyTexture(s.second);
        }
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
}

/**
 * DEVELOPER: Michael Goll
 * DESIGNER:  Michael Goll
 * DATE:      March 14, 2017
 * load all sprites sheets
 */
void Renderer::loadSprites() {
    logv("Loading Sprites...\n");
    //Main game screen
    createTexture(TEXTURES::MAIN, MAIN_SCREEN);
    createTexture(TEXTURES::TEXTBOX, TEXTBOX_TEXTURE);
    //createTexture(LOBBY_SCREEN);

    //-------- map textures --------
    createTexture(TEXTURES::BARREN, TEXTURE_BARREN);     //barren dirt
    createTexture(TEXTURES::DEAD_GRASS, TEXTURE_MIDDLE); //dead grass
    createTexture(TEXTURES::TERRAFORMED, TEXTURE_DIRT);  //terraformed
    createTexture(TEXTURES::CONCRETE, REPLACE_ME);     //concrete, temporary texture for now

    //-------- map object textures --------
    //nature
    //comsumables
    //shops
    createTexture(TEXTURES::MAP_OBJECTS, MAP_OBJECTS);

    //-------- weapon textures --------
    createTexture(TEXTURES::WEAPONS, REPLACE_ME); //temporary, will be replaced later

    //-------- marine textures --------
    createTexture(TEXTURES::MARINE, TEMP_MARINE_TEXTURE);

    //-------- zombie textures --------
    //baby
    //createTexture(ZOMBIE_BABYZ);
    createTexture(TEXTURES::BABY_ZOMBIE, TEMP_ZOMBIE_TEXTURE);
    //digger
    createTexture(TEXTURES::DIGGER_ZOMBIE, ZOMBIE_DIGGER);
    //boss
    createTexture(TEXTURES::BOSS_ZOMBIE, ZOMBIE_BOSS);
}

/**
 * DEVELOPER: Michael Goll
 * DESIGNER:  Michael Goll
 * DATE:      March 14, 2017
 *
 * REVISED: Isaac Morneau, March 25, 2017
 *      changed pointers and static to references
 *
 * Loads a font from a TTF file
 */
TTF_Font* Renderer::loadFont(const std::string& filePath, const int size) {
    TTF_Font * font = nullptr;

    if ((font = TTF_OpenFont(filePath.c_str(), size)) == nullptr) {
        logv(TTF_GetError());
        return nullptr;
    }
    return font;
}

/**
 * DEVELOPER: Michael Goll
 * DESIGNER:  Michael Goll
 * DATE:      March 14, 2017
 * creates a texture and adds it to the array
 */
void Renderer::createTexture(const int index, const std::string& filePath) {

    SDL_Surface * surface = IMG_Load(filePath.c_str());

    if (surface == nullptr) {
        logv("Cannot create surface, error: %s\n", SDL_GetError());
    } else {
        //gets rid of the white in the image
        SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGB(surface->format, 0, 0xFF, 0xFF));

        //create texture
        SDL_Texture * texture = SDL_CreateTextureFromSurface(renderer, surface);

        if (texture == nullptr) {
            logv("Cannot create texture, error: %s\n", SDL_GetError());
        } else {
            //add the texture to the array
            sprites.insert({static_cast<int>(index), texture});
            SDL_FreeSurface(surface);
        }
    }
}

void Renderer::createTexture(const TEXTURES index, const std::string& filePath) {
    createTexture(static_cast<int>(index), filePath);
}

/**
 * DEVELOPER: Michael Goll
 * DESIGNER:  Michael Goll
 * DATE:      March 20, 2017
 * creates a texture, adds it to the map and returns its ID
 */
int Renderer::createTempTexture(const std::string& filePath) {
    createTexture(tempIndex, filePath);
    return tempIndex++;
}

/**
 * DEVELOPER: Michael Goll
 * DESIGNER:  Michael Goll
 * DATE:      March 14, 2017
 * creates a texture out of text
 * returns a 0 on error, otherwise returns the id where it is stored
 */
void Renderer::createText(const TEXTURES index, TTF_Font* font, const std::string& text, const SDL_Color& colour) {
    SDL_Surface * textSurface = TTF_RenderText_Solid(font, text.c_str(), colour);

    if (textSurface == nullptr) {
        logv("Cannot create text surface, error: %s\n", TTF_GetError());
    } else {

        SDL_Texture * texture = SDL_CreateTextureFromSurface(renderer, textSurface);

        if (texture == nullptr) {
            logv("Cannot create texture, error: %s\n", SDL_GetError());
            SDL_DestroyTexture(texture);
        } else {
            sprites.insert({static_cast<int>(index), texture});
            SDL_FreeSurface(textSurface);
        }
    }
}

/**
 * DEVELOPER: Michael Goll
 * DESIGNER:  Michael Goll
 * DATE:      March 20, 2017
 * creates a texture out of text, used for temporary texts (usernames, ect)
 * returns a 0 on error, otherwise returns the id where it is stored
 */
int Renderer::createTempText(TTF_Font * font, const std::string& text, const SDL_Color& colour) {
    SDL_Surface * textSurface = TTF_RenderText_Solid(font, text.c_str(), colour);

    if (textSurface == nullptr) {
        logv("Cannot create text surface, error: %s\n", TTF_GetError());
    } else {

        SDL_Texture * texture = SDL_CreateTextureFromSurface(renderer, textSurface);

        if (texture == nullptr) {
            logv("Cannot create texture, error: %s\n", SDL_GetError());
            SDL_DestroyTexture(texture);
        } else {
            sprites.insert({static_cast<int>(tempIndex), texture});
            SDL_FreeSurface(textSurface);
            return tempIndex++;
        }
    }
    return 0;
}

/**
 * DEVELOPER: Michael Goll
 * DESIGNER:  Michael Goll
 * DATE:      March 14, 2017
 * sets the game's renderer
 */
void Renderer::setRenderer() {
    if ((renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED)) == nullptr) {
        logv("Renderer could not be created\n");
    }
}

/**
 * DEVELOPER: Michael Goll
 * DESIGNER:  Michael Goll
 * DATE:      March 14, 2017
 * sets the window
 */
void Renderer::setWindow(SDL_Window * win) {
    window = win;
    setRenderer();
}



/**
 * DEVELOPER: Isaac Morneau
 * DESIGNER:  Isaac Morneau
 * DATE:      March 14, 2017
 * wraps the call using a texture to an int as
 * texture is a scoped enum and is no longer auto converted
 */
void Renderer::render(const SDL_Rect& dest, const TEXTURES spriteType, const SDL_Rect& clip,
        double angle, const SDL_Point* center, const SDL_RendererFlip flip) {
    render(dest, static_cast<int>(spriteType), clip, angle, center, flip);
}
/**
 * DEVELOPER: Michael Goll
 * DESIGNER:  Michael Goll
 * DATE:      March 14, 2017
 * REVISION:  Isaac Morneau, March 26, 2017
 *      added duplicates to autowrap textures to ints
 * renders an object
 */
void Renderer::render(const SDL_Rect& dest, const int spriteType, const SDL_Rect& clip,
        double angle, const SDL_Point* center, const SDL_RendererFlip flip) {
    //Render to screen
    SDL_RenderCopyEx(renderer, getTexture(spriteType), &clip, &dest, angle,
            center, flip);
}


/**
 * DEVELOPER: Isaac Morneau
 * DESIGNER:  Isaac Morneau
 * DATE:      March 14, 2017
 * wraps the call using a texture to an int as
 * texture is a scoped enum and is no longer auto converted
 */
void Renderer::render(const SDL_Rect& dest, const TEXTURES spriteType, double angle,
        const SDL_Point* center, const SDL_RendererFlip flip) {
    render(dest, static_cast<int>(spriteType), angle, center, flip);
}

/**
 * DEVELOPER: Michael Goll
 * DESIGNER:  Michael Goll
 * DATE:      March 14, 2017
 * renders an object
 */
void Renderer::render(const SDL_Rect& dest, const int spriteType, double angle,
        const SDL_Point* center, const SDL_RendererFlip flip) {
    //Render to screen
    SDL_RenderCopyEx(renderer, getTexture(spriteType), nullptr, &dest, angle,
            center, flip);
}

/**
 * DEVELOPER: Michael Goll
 * DESIGNER:  Michael Goll
 * DATE:      March 14, 2017
 * returns the sprite or sprite sheet that the object is looking to render
 */
SDL_Texture * Renderer::getTexture(int spriteType) {
    auto texture = sprites.find(spriteType);

    if (texture != sprites.end()) {
        return texture->second;
    }

    return nullptr;
}
