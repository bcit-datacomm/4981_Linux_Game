#include <stdio.h>
#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h> 
#include <SDL2/SDL_ttf.h>
#include "GameStateMatch.hpp"
#include "LTimer.hpp"
#include "LTexture.hpp"
#include "Frame.hpp"

bool GameStateMatch::load()
{
	
	bool success = true;
	
	//Open the font
	this->frameFont = TTF_OpenFont( "assets/kenpixelsquare.ttf", 28 );
	if( this->frameFont == NULL )
	{
		printf( "Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError() );
		success = false;
	}
	
	return success;
}

void GameStateMatch::loop()
{
	//The frames per second timer
	LTimer fpsTimer;

	//The frames per second cap timer
	LTimer capTimer;
	
	float avgFPS = 0;

	//Start counting frames per second
	unsigned long countedFrames = 0;
	fpsTimer.start();
	
	// State Loop
	while (this->play)
	{
		//Start cap timer
		capTimer.start();
		
		//Calculate and correct fps
		avgFPS = countedFrames / ( fpsTimer.getTicks() / 1000.f );
		
		//Set FPS text to be rendered
		this->frameTimeText.str( "" );
		this->frameTimeText << std::fixed << std::setprecision(0) << "FPS: " << avgFPS; 
		
		// Process frame
		this->sync();
		this->handle();
		this->update();
		this->render();
		
		++countedFrames;

		//If frame finished early
		int frameTicks = capTimer.getTicks();
		if( frameTicks < SCREEN_TICK_PER_FRAME )
		{
			//Wait remaining time
			SDL_Delay( SCREEN_TICK_PER_FRAME - frameTicks );
		}
		
	}
}

void GameStateMatch::sync()
{

}

void GameStateMatch::handle()
{
	//Handle events on queue
	while( SDL_PollEvent( &this->event ))
	{
		/* We are only worried about SDL_KEYDOWN and SDL_KEYUP events */
   		switch( this->event.type ){
      	case SDL_KEYDOWN:
        	printf( "Key press detected\n" );
				SDL_Delay(100);
        	break;
      	case SDL_KEYUP:
        	printf( "Key release detected\n" );
        	break;
		case SDL_QUIT:
			play = false;
			break;	
      	default:
        	break;
    	}
	}
}

void GameStateMatch::update()
{

}

void GameStateMatch::render()
{
	SDL_Color textColor = { 0, 0, 0, 255 };
	
	//Render text
	if( !this->frameFPSTextTexture.loadFromRenderedText( this->frameTimeText.str().c_str(),
											  textColor, this->game->renderer, this->frameFont ) )
	{
		printf( "Unable to render FPS texture!\n" );
	}

	//Clear screen
	SDL_SetRenderDrawColor( this->game->renderer, 0xFF, 0xFF, 0xFF, 0xFF );
	SDL_RenderClear( this->game->renderer );

	//Render textures
	this->frameFPSTextTexture.render(this->game->renderer, 
									 ( SCREEN_WIDTH - this->frameFPSTextTexture.getWidth() ), 0 );
	
	//Update screen
	SDL_RenderPresent( this->game->renderer );
	
}

	