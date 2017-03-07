#ifndef GAMESTATE_H
#define GAMESTATE_H
#include <SDL2/SDL.h>
#include "Game.h"

class GameState {
public:

    virtual bool load() = 0;
    virtual void loop() = 0;

    // Pointer to game parent
    Game& game;

    // Event handler
    SDL_Event event;

    //Main loop flag
    bool play = true;

    GameState(Game& g): game(g){};
    virtual ~GameState() {};

private:

    virtual void handle() = 0;
    virtual void update(const float& delta) = 0;
    virtual void render() = 0;
    virtual void sync() = 0;

};

#endif
