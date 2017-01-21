#ifndef GAMESTATE_HPP
#define GAMESTATE_HPP
#include<SDL2/SDL.h>
#include "Game.hpp"

class GameState {
private:

	virtual void handle() = 0;
	virtual void update() = 0;
	virtual void render() = 0;
	virtual void sync() = 0;
	
public:

	virtual bool load() = 0;
	virtual void loop() = 0;
	
	Game* game = NULL;
	
	GameState(Game* g) { this->game = g; };
	virtual ~GameState() {};
	
};

#endif