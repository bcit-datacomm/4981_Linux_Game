#ifndef GAME_HPP
#define GAME_HPP
#include<SDL2/SDL.h>
#include <SDL2/SDL_image.h> 
#include <SDL2/SDL_ttf.h>

class GameState;

class Game {
private:
	
	GameState* state = NULL;
	
public:
	
	SDL_Window* window = NULL;
	SDL_Surface* screenSurface = NULL;
	SDL_Renderer* renderer = NULL;
	
	bool init();
	bool loadMedia();
	void run();
	void close();
	
};



#endif