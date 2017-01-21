#include <stdio.h>
#include <iostream>
#include <string>
#include "Game.hpp"


int main()
{
	Game* game = new Game();
	
	//Start up SDL and create window
	if(game->init() && game->loadMedia())
	{
		game->run();
	}
	else
	{
		printf( "Failed to start!\n" );
	}

	//Free resources and close SDL
	game->close();
	
	delete game;
	printf( "Exit\n" );
	return 0;
}
