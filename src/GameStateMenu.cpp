#include <stdio.h>
#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "GameStateMenu.h"
#include "LTimer.h"
#include "LTexture.h"
#include "Window.h"

bool GameStateMenu::load() {

	bool success = true;

	//Open the font
	this->frameFont = TTF_OpenFont( "assets/fonts/kenpixelsquare.ttf", 50);
	if ( this->frameFont == NULL ) {
		printf( "Failed to load font! SDL_ttf Error: %s\n", TTF_GetError() );
		success = false;
	}

	this->menuFont = TTF_OpenFont( "assets/fonts/Overdrive Sunset.otf", 100);
	if ( this->frameFont == NULL ) {
		printf( "Failed to load font! SDL_ttf Error: %s\n", TTF_GetError() );
		success = false;
	}

	this->level = new Level();
	if (!this->level->levelTexture.loadFromFile("assets/TitleScreen_Marz.png", this->game->renderer)) {
		printf("Failed to load the level texture!\n");
	} else {
		this->level->levelTexture.setDimensions(this->game->window->getWidth(), this->game->window->getHeight());	
	}

	this->camera = new Camera(this->game->window->getWidth(), this->game->window->getHeight());


	return success;
}

void GameStateMenu::loop() {
	//The frames per second timer
	LTimer fpsTimer;

	//The frames per second cap timer
	LTimer capTimer;

	//Keeps track of time between steps
	LTimer stepTimer;

	//Start counting frames per second
	unsigned long countedFrames = 0;
	float avgFPS = 0;
	fpsTimer.start();

	// State Loop
	while (this->play) {
		//Start cap timer
		capTimer.start();

		//Calculate and correct fps
		avgFPS = countedFrames / ( fpsTimer.getTicks() / 1000.f );

		//Set FPS text to be rendered
		this->frameTimeText.str( "" );
		this->frameTimeText << std::fixed << std::setprecision(0) << "FPS: " << avgFPS;

		// Process frame
		this->handle();	// Handle user input
		this->update(stepTimer.getTicks() / 1000.f); // Update state values
		stepTimer.start(); //Restart step timer
		this->sync();	// Sync game to server
		this->render();	// Render game state to window

		++countedFrames;

		//If frame finished early
		int frameTicks = capTimer.getTicks();
		if ( frameTicks < SCREEN_TICK_PER_FRAME ) {
			//Wait remaining time
			SDL_Delay( SCREEN_TICK_PER_FRAME - frameTicks );
		}

		SDL_RenderPresent(this->game->renderer);

	}
}

void GameStateMenu::sync() {

}

/**
* Function: handle
* 
* Date: 
* JF: February 8, 2017: added handler for window resizing event
* 
* Designer:
* 
* Programmer:
* Jacob Frank
* 
* Interface: handle()
* 
* Returns: void
* 
* Notes:
* Function handles events occuring within the window to control
* functionality of the main menu
*/
void GameStateMenu::handle() {
	int x,y;

	//Handle events on queue
	while ( SDL_PollEvent( &this->event )) {
		this->game->window->handleEvent(this->event);
   		switch ( this->event.type ) {
	      	case SDL_KEYDOWN:
	        	switch ( this->event.key.keysym.sym ) {
					case SDLK_ESCAPE:
						play = false;
						break;
					default:
		                break;
				}
	        	break;
	      	case SDL_KEYUP:
	       		switch ( this->event.key.keysym.sym ) {
					default:
		               	break;
				}
	        	break;
	        case SDL_MOUSEMOTION:
	        	x = this->event.motion.x;
	        	y = this->event.motion.y;
	        	for (int i = 0; i < NUM_MENU_ITEMS; i++) {
	        		if (x >= pos[i].x && x <= pos[i].x + this->menuTextTextures[i].getWidth() &&
	        			y >= pos[i].y && y <= pos[i].y + this->menuTextTextures[i].getHeight()) {
	        			if (!selected[i]) {
	        				selected[i] = true;
	        			}
	        		} else {
	        			if (selected[i]) {
	        				selected[i] = false;
	        			}	        			
	        		}
	        	}
	        	break;
	        case SDL_MOUSEBUTTONDOWN:
	        	x = this->event.button.x;
	        	y = this->event.button.y;
	        	for (int i = 0; i < NUM_MENU_ITEMS; i++) {
	        		if (x >= pos[i].x && x <= pos[i].x + this->menuTextTextures[i].getWidth() &&
	        			y >= pos[i].y && y <= pos[i].y + this->menuTextTextures[i].getHeight()) {
	        			this->update(i);
	        		}
	        		play = false;
	        	}

	        	break;
	        case SDL_WINDOWEVENT:
	        	switch (this->event.window.event) {
		    		case SDL_WINDOWEVENT_RESIZED:
		    			this->level->levelTexture.setDimensions(this->game->window->getWidth(), 
		    													this->game->window->getHeight());
		    			break;
		        }
	        	break;
			case SDL_QUIT:
				play = false;
				break;
	      	default:
	        	break;
    	}
	}
}

void GameStateMenu::update(const float& delta) {
	
	// TEMP: need to handle changing states better (use correct numbering in Game.cpp)
	//Add a switch statement instead
	if(delta == 0) {
		this->game->stateID = 2;
	}
}

void GameStateMenu::render() {
	//Only draw when not minimized
	if ( !this->game->window->isMinimized() ) {

		//Clear screen
		SDL_SetRenderDrawColor( this->game->renderer, 0xFF, 0xFF, 0xFF, 0xFF );
		SDL_RenderClear( this->game->renderer );
		int windowWidth = this->game->window->getWidth();
		int windowHeight = this->game->window->getHeight();

		//Render textures
		this->level->levelTexture.render(this->game->renderer, 
										 0-this->camera->getX(),
										 0-this->camera->getY());
	
		SDL_Color textColor = { 0, 0, 0, 255 };


		//Render FPS text
		if ( !this->frameFPSTextTexture.loadFromRenderedText( this->frameTimeText.str().c_str(),
											  textColor, this->game->renderer, this->frameFont ) ) {
			printf( "Unable to render FPS texture!\n" );
		}		
		this->frameFPSTextTexture.render(this->game->renderer, (windowWidth - this->frameFPSTextTexture.getWidth()), 0);


		//Render Menu text
		for (int i = 0; i < NUM_MENU_ITEMS; i++) {
			if(selected[i]) {
				if ( !this->menuTextTextures[i].loadFromRenderedText( this->menuItems[i],
											  fontColors[1], this->game->renderer, this->menuFont ) ) {
					printf( "Unable to render Join texture!\n" );				
				}
			} else {
				if ( !this->menuTextTextures[i].loadFromRenderedText( this->menuItems[i],
											  fontColors[0], this->game->renderer, this->menuFont ) ) {
					printf( "Unable to render Join texture!\n" );				
				}
			}
			this->menuTextTextures[i].render(this->game->renderer, pos[i].x, pos[i].y);
		}

		//Position the menu text
		pos[0].x = windowWidth/2 - this->menuTextTextures[0].getWidth()/2;
		pos[0].y = windowHeight/2 - this->menuTextTextures[0].getHeight();
		pos[1].x = windowWidth/2 - this->menuTextTextures[1].getWidth()/2;
		pos[1].y = windowHeight/2 + this->menuTextTextures[1].getHeight();

		
		//Update screen
		SDL_RenderPresent( this->game->renderer );
	}
}

GameStateMenu::~GameStateMenu() {
	
	// Free texture and font
	this->frameFPSTextTexture.free();
	this->menuTextTextures[1].free();
	this->menuTextTextures[0].free();
	TTF_CloseFont(this->frameFont);
	TTF_CloseFont(this->menuFont);
	this->menuFont = NULL;
	this->frameFont = NULL;

}
 