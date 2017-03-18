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
    Game() : window(), state(nullptr), renderer(nullptr), screenSurface(nullptr) {};
    ~Game();

    bool init();
    auto& getWindow() {return window;}
    auto getRenderer() const {return renderer;}
    auto getSurface() const {return screenSurface;}
    void setStateID(const unsigned int id) {stateID = id;}
    bool loadMedia();
    SDL_Surface *loadSurface(std::string path);
    SDL_Texture *loadTexture(std::string path);
    void run();
    void loadState();

private:
    // Game window
    Window window;

    // Current game state
    std::unique_ptr<GameState> state;//stays as a pointer cause we need to be able to switch between match and menu states

    SDL_Renderer *renderer;
    SDL_Surface *screenSurface;

    unsigned int stateID = 1; // Starting game state id
};

#endif
