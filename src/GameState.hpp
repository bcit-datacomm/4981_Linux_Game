#ifndef GAMESTATE_HPP
#define GAMESTATE_HPP
#include<SDL2/SDL.h>
#include "Game.hpp"

class GameState {
private:

	virtual void handle() = 0;
	virtual void update(const float& delta) = 0;
	virtual void render() = 0;
	virtual void sync() = 0;
	
	
public:

	virtual bool load() = 0;
	virtual void loop() = 0;
	
	// Pointer to game parent
	Game* game = NULL;
	
	// Event handler
	SDL_Event event;
	
	//Main loop flag
	bool play = true;
	
	GameState(Game* g) { this->game = g; };
	virtual ~GameState() {};
	
};

#endif