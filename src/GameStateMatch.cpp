#include <stdio.h>
#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "GameStateMatch.h"
#include "LTimer.h"
#include "LTexture.h"
#include "Window.h"

bool GameStateMatch::load() {

	bool success = true;

	//Open the font
	this->frameFont = TTF_OpenFont( "assets/fonts/kenpixelsquare.ttf", 28 );
	if ( this->frameFont == NULL ) {
		printf( "Failed to load font! SDL_ttf Error: %s\n", TTF_GetError() );
		success = false;
	}

	this->level = new Level();
	if (!this->level->levelTexture.loadFromFile("assets/texture/checkerboard.png", this->game->renderer)) {
		printf("Failed to load the level texture!\n");
		success = false;
	} else {
		this->level->levelTexture.setDimensions(2000, 2000);
	}

	unsigned int playerMarineID = GameManager::instance()->createMarine();

	// Create Dummy Entitys
	success = GameManager::instance()->createMarine(this->game->renderer, 1500, 1500);
	success = GameManager::instance()->createZombie(this->game->renderer, 100, 100);
	success = GameManager::instance()->createZombie(this->game->renderer, 700, 700);
	success = GameManager::instance()->createTurret(this->game->renderer, 1000, 500);
	
	this->base = new Base();
	if (!this->base->texture.loadFromFile("assets/texture/base.png", this->game->renderer)) {
		printf("Failed to load the base texture!\n");
		success = false;
	}
	GameManager::instance()->addObject(this->base);
	Point newPoint = this->base->getSpawnPoint();

	this->player = new Player();
	this->player->setControl(GameManager::instance()->getMarine(playerMarineID));
	this->player->marine->setPosition(newPoint.first, newPoint.second);

	if (!this->player->marine->texture.loadFromFile("assets/texture/arrow.png", this->game->renderer)) {
		printf("Failed to load the player texture!\n");
		success = false;
	}

	this->camera = new Camera(this->game->window->getWidth(), this->game->window->getHeight());


	return success;
}

void GameStateMatch::loop() {
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

	}
}

void GameStateMatch::sync() {

}

void GameStateMatch::handle() {
	const Uint8 *state = SDL_GetKeyboardState(NULL); // Keyboard state
	// Handle movement input
	this->player->handleKeyboardInput(state);
	this->player->handleMouseUpdate(this->game->window, this->camera->getX(), this->camera->getY());
	//Handle events on queue
	while ( SDL_PollEvent( &this->event )) {
		this->game->window->handleEvent(this->event);
   		switch( this->event.type ) {
		case SDL_WINDOWEVENT:
			this->camera->setViewSize(this->game->window->getWidth(), this->game->window->getHeight());
			break;
		case SDL_MOUSEWHEEL:
			this->player->handleMouseWheelInput(&(this->event));
			break;
        case SDL_MOUSEBUTTONDOWN:
			if (this->event.button.button == SDL_BUTTON_RIGHT) {
				this->player->handlePlacementClick(this->game->renderer);
			}
            break;
      	case SDL_KEYDOWN:
        	switch( this->event.key.keysym.sym ) {
			case SDLK_ESCAPE:
				play = false;
				break;
			case SDLK_b:
				this->player->placeBarricade(this->game->renderer);
				break;
			default:
                break;
			}
        	break;
      	case SDL_KEYUP:
       		switch( this->event.key.keysym.sym ) {
			default:
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

void GameStateMatch::update(const float& delta) {
	GameManager::instance()->updateCollider();

	// Move player
	GameManager::instance()->updateMarines(delta);
	GameManager::instance()->updateZombies(delta);

	// Move Camera
	this->camera->move(this->player->marine->getX(), this->player->marine->getY());


}

void GameStateMatch::render() {
	//Only draw when not minimized
	if ( !this->game->window->isMinimized() ) {

		//Clear screen
		SDL_SetRenderDrawColor( this->game->renderer, 0xFF, 0xFF, 0xFF, 0xFF );
		SDL_RenderClear( this->game->renderer );

		//Render textures
		this->level->levelTexture.render(this->game->renderer,
										 0-this->camera->getX(),
										 0-this->camera->getY());

		//renders objects in game
		GameManager::instance()->renderObjects(this->game->renderer, this->camera->getX(), this->camera->getY());

		SDL_Color textColor = { 0, 0, 0, 255 };

		//Render text
		if ( !this->frameFPSTextTexture.loadFromRenderedText( this->frameTimeText.str().c_str(),
											  textColor, this->game->renderer, this->frameFont ) ) {
			printf( "Unable to render FPS texture!\n" );
		}

		this->frameFPSTextTexture.render(this->game->renderer,
								( this->game->window->getWidth() - this->frameFPSTextTexture.getWidth() ), 0);

		//Update screen
		SDL_RenderPresent( this->game->renderer );
	}
}

GameStateMatch::~GameStateMatch() {
	
	// Free texture and font
	delete GameManager::instance();
	delete this->camera;
	delete this->player;
	delete this->level;
	this->frameFPSTextTexture.free();
	TTF_CloseFont(this->frameFont);
	this->frameFont = NULL;

}
