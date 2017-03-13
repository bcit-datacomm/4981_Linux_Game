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

GameStateMatch::GameStateMatch(Game& g,  int gameWidth, int gameHeight) : GameState(g), player(),
                               level(),  base(), camera(gameWidth,gameHeight){

}

bool GameStateMatch::load() {

    bool success = true;

    //Open the font
    frameFont = TTF_OpenFont( "assets/fonts/kenpixelsquare.ttf", 28 );
    if ( frameFont == NULL ) {
        printf( "Failed to load font! SDL_ttf Error: %s\n", TTF_GetError() );
        success = false;
    }

    //level = new Level();
    if (!level.levelTexture.loadFromFile("assets/texture/checkerboard.png", game.renderer)) {
        printf("Failed to load the level texture!\n");
        success = false;
    } else {
        level.levelTexture.setDimensions(2000, 2000);
    }

    unsigned int playerMarineID = GameManager::instance()->createMarine();

    // Create Dummy Entitys
    GameManager::instance()->createMarine(game.renderer, 1500, 1500);
    GameManager::instance()->createZombie(game.renderer, 100, 100);
    GameManager::instance()->createZombie(game.renderer, 700, 700);
    GameManager::instance()->createZombie(game.renderer, 800, 1400);
    GameManager::instance()->createZombie(game.renderer, 100, 1400);
    GameManager::instance()->createZombie(game.renderer, 800, 100);
    GameManager::instance()->createTurret(game.renderer, 1000, 500);
    GameManager::instance()->createWeaponDrop(game.renderer, 1800, 1700);

    //base = Base();
    if (!base.texture.loadFromFile("assets/texture/base.png", game.renderer)) {
        printf("Failed to load the base texture!\n");
        success = false;
    }
    GameManager::instance()->addObject(base);
    Point newPoint = base.getSpawnPoint();

    //player = new Player();
    player.setControl(GameManager::instance()->getMarine(playerMarineID));
    player.marine->setPosition(newPoint.first, newPoint.second);

    if (!player.marine->texture.loadFromFile("assets/texture/arrow.png", game.renderer)) {
        printf("Failed to load the player texture!\n");
        success = false;
    }

    //camera = Camera(game.window.getWidth(), game.window.getHeight());


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
    while (play) {
        //Start cap timer
        capTimer.start();

        //Calculate and correct fps
        avgFPS = countedFrames / ( fpsTimer.getTicks() / 1000.f );

        //Set FPS text to be rendered
        frameTimeText.str( "" );
        frameTimeText << std::fixed << std::setprecision(0) << "FPS: " << avgFPS;

        // Process frame
        handle();    // Handle user input
        update(stepTimer.getTicks() / 1000.f); // Update state values
        stepTimer.start(); //Restart step timer
        sync();    // Sync game to server
        render();    // Render game state to window

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
    player.handleKeyboardInput(state);
    player.handleMouseUpdate(game.window, camera.getX(), camera.getY());
    //Handle events on queue
    while ( SDL_PollEvent( &event )) {
        game.window.handleEvent(event);
           switch( event.type ) {
        case SDL_WINDOWEVENT:
            camera.setViewSize(game.window.getWidth(), game.window.getHeight());
            break;
        case SDL_MOUSEWHEEL:
            player.handleMouseWheelInput(&(event));
            break;
        case SDL_MOUSEBUTTONDOWN:
            if (event.button.button == SDL_BUTTON_RIGHT) {
                player.handlePlacementClick(game.renderer);
            }
            break;
      	case SDL_KEYDOWN:
        	switch( this->event.key.keysym.sym ) {
			case SDLK_ESCAPE:
				play = false;
				break;
			case SDLK_b:
				player.handleTempBarricade(game.renderer);
				break;
			default:
                break;
            }
            break;
          case SDL_KEYUP:
               switch( event.key.keysym.sym ) {
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
	GameManager::instance()->updateTurrets(delta);

    // Move Camera
    camera.move(player.marine->getX(), player.marine->getY());


}

void GameStateMatch::render() {
    //Only draw when not minimized
    if ( !game.window.isMinimized() ) {

        //Clear screen
        SDL_SetRenderDrawColor( game.renderer, 0xFF, 0xFF, 0xFF, 0xFF );
        SDL_RenderClear( game.renderer );

        //Render textures
        level.levelTexture.render(game.renderer, 0-camera.getX(), 0-camera.getY());

        //renders objects in game
        GameManager::instance()->renderObjects(game.renderer, camera.getX(), camera.getY());

        SDL_Color textColor = { 0, 0, 0, 255 };

        //Render text
        if ( !frameFPSTextTexture.loadFromRenderedText( frameTimeText.str().c_str(),
                textColor, game.renderer, frameFont ) ) {
            printf( "Unable to render FPS texture!\n" );
        }

        frameFPSTextTexture.render(game.renderer,
            (game.window.getWidth() - frameFPSTextTexture.getWidth()), 0);

        //Update screen
        SDL_RenderPresent( game.renderer );
    }
}

GameStateMatch::~GameStateMatch() {

    // Free texture and font
    delete GameManager::instance();
    frameFPSTextTexture.free();
    TTF_CloseFont(frameFont);
    frameFont = NULL;

}
