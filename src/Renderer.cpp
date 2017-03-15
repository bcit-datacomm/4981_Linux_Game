#include "Renderer.h"
#include "Window.h"

/* DEVELOPER: Michael Goll
** DESIGNER:  Michael Goll
** DATE:      March 14, 2017
*/

Renderer * Renderer::rInstance = nullptr;
SDL_Renderer * Renderer::_renderer = nullptr;
SDL_Window * Renderer::_window = nullptr;

std::array<SDL_Texture *, TOTAL_SPRITES> Renderer::_sprites = {0};
int Renderer::_index = 0;

Renderer::~Renderer() {
    for (const auto& s : _sprites) {
        if (s != nullptr) {
            SDL_DestroyTexture(s);
        }
    }

    SDL_DestroyRenderer(_renderer);
    SDL_DestroyWindow(_window);
    free(rInstance);
}

//returns the instance if it exists, otherwise creates one
Renderer * Renderer::instance() {
    if (!rInstance) {
        rInstance = new Renderer;
    }
    return rInstance;
}

//load all sprites sheets
void Renderer::loadSprites() {
    //Main game screen
    //createTexture(MAIN_SCREEN);
    //createTexture(LOBBY_SCREEN);

    //-------- map textures --------
    createTexture(TEXTURE_BARREN); //barren dirt
    createTexture(TEXTURE_MIDDLE); //dead grass
    createTexture(TEXTURE_DIRT);   //terraformed
    createTexture(REPLACE_ME);    //concrete

    //-------- map object textures --------
        //nature
        //comsumables
        //shops
    createTexture(MAP_OBJECTS);

    //-------- weapon textures --------
    createTexture(REPLACE_ME);

    //-------- marine textures --------
    createTexture(TEMP_MARINE_TEXTURE);

    //-------- zombie textures --------
    //baby
    //createTexture(ZOMBIE_BABYZ);
    createTexture(TEMP_ZOMBIE_TEXTURE);
    //digger
    createTexture(ZOMBIE_DIGGER);
    //boss
    createTexture(ZOMBIE_BOSS);
}

//creates a texture and adds it to the array
void Renderer::createTexture(std::string filePath) {
    printf("Creating texture: %s\n", filePath.c_str());

    SDL_Surface * surface = IMG_Load(filePath.c_str());

    if (surface == nullptr) {
        printf("Cannot create surface, error: %s\n", SDL_GetError());
    } else {
        //gets rid of the white in the image
        SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGB(surface->format, 0, 0xFF, 0xFF));

        //create texture
        SDL_Texture * texture = SDL_CreateTextureFromSurface(_renderer, surface);

        if (texture == nullptr) {
            printf("Cannot create texture, error: %s\n", SDL_GetError());
        } else {
            //add the texture to the array
            _sprites[_index++] = texture;
        }

        SDL_FreeSurface(surface);
    }
}

//sets the game's renderer
void Renderer::setRenderer() {
    if ((_renderer = SDL_CreateRenderer( _window, -1, SDL_RENDERER_ACCELERATED)) == nullptr) {
        printf("Renderer could not be created\n");
    }
}

//sets the window
void Renderer::setWindow(SDL_Window * window) {
    _window = window;
    setRenderer();
}

//renders an object
void Renderer::render(int x, int y, int w, int h, TEXTURES spriteType, SDL_Rect* clip, double angle,
                      SDL_Point* center, SDL_RendererFlip flip) {

    //Set rendering space and render to screen
    SDL_Rect renderQuad = { x, y, w, h };

    //Set clip rendering dimensions
    if( clip != NULL ) {
        renderQuad.w = clip->w;
        renderQuad.h = clip->h;
    }

    //Render to screen
    SDL_RenderCopyEx(_renderer, getTexture(static_cast<int>(spriteType)), clip, &renderQuad, angle,
                     center, flip);
}
