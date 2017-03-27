#ifndef GAME_H
#define GAME_H
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <memory>
#include "../view/Window.h"
#include "GameState.h"

class Game {
public:
    Game():window(){};
    ~Game();

    // Game window
    Window window;
    SDL_Surface* screenSurface = nullptr;
    unsigned int stateID = 1; // Starting game state id

    bool init();
    void setStateID(const unsigned int id) {stateID = id;}
    bool loadMedia();
    void run();
    void loadState();
    void close();

private:
    // Current game state
    std::unique_ptr<GameState> state;//stays as a pointer cause we need to be able to switch between match and menu states
};

#endif
