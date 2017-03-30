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
    Game() : window(), stateID(1), screenSurface(nullptr){};
    ~Game();

    bool init();
    void setStateID(const unsigned int id) {stateID = id;}
    bool loadMedia();
    void run();
    void loadState();
    void close();
    Window& getWindow() {return window;}

private:
    //Current game state
    std::unique_ptr<GameState> state;//stays as a pointer cause we need to be able to switch between match and menu states
    Window window; //Game window
    unsigned int stateID; //Starting game state id
    SDL_Surface *screenSurface;
};

#endif
