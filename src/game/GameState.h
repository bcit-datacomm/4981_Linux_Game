/**
* Date: Jan. 20, 2017
* Author: Jacob McPhail
* Modified: ---
* Description: 
*   Parant class for game states.
*/
#ifndef GAMESTATE_H
#define GAMESTATE_H
#include <SDL2/SDL.h>

class Game;

class GameState {
public:
    GameState(Game& g) : game(g) {};
    virtual ~GameState() = default;

    virtual bool load() = 0;
    virtual void loop() = 0;

    auto& getGame() const {return game;}
    auto& getEventHandler() const {return event;}
    auto getPlayFlag() const {return play;}

protected:
    //Reference to game parent
    Game& game;

    //Event handler
    SDL_Event event;
    //
    //Main loop flag
    bool play = true;

private:
    virtual void handle() = 0;
    virtual void update(const float delta) = 0;
    virtual void render() = 0;
    virtual void sync() = 0;

};

#endif
