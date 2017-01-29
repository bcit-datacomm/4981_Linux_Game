#ifndef GAME_HPP
#define GAME_HPP
#include<SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "Window.hpp"

class GameState;

class Game {
private:

	// Current game state
	GameState* state = NULL;

public:

	// Game window
	Window* window = NULL;
	SDL_Renderer* renderer = NULL;
	SDL_Surface* screenSurface = NULL;


	bool init();
	bool loadMedia();
	SDL_Surface* loadSurface( std::string path );
	SDL_Texture* loadTexture( std::string path );
	void run();
	void close();

};



#endif
