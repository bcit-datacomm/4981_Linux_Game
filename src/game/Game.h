#ifndef GAME_H
#define GAME_H
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "../view/Window.h"

class GameState;

class Game {
public:
    Game():window(){};

    // Game window
    Window window;
    SDL_Renderer* renderer = NULL;
    SDL_Surface* screenSurface = NULL;

    unsigned int stateID = 1; // Starting game state id

    bool init();
    bool loadMedia();
    SDL_Surface* loadSurface( std::string path );
    SDL_Texture* loadTexture( std::string path );
    void run();
    void loadState();
    void close();

private:

    // Current game state
    GameState* state = nullptr;//stays as a pointer cause we need to be able to switch between match and menu states

};

#endif
