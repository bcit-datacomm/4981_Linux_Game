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
#include "Renderer.h"
#include "SpriteTypes.h"

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

    //loads the sprites
    printf("Loading Sprites\n");
    Renderer::instance()->loadSprites();

    unsigned int playerMarineID = GameManager::instance()->createMarine();

    // Create Dummy Entitys
    GameManager::instance()->createMarine(Renderer::instance()->getRenderer(), 0, 0);
    GameManager::instance()->createZombie(Renderer::instance()->getRenderer(), 800, 800);
    GameManager::instance()->createZombie(Renderer::instance()->getRenderer(), 700, 700);
    GameManager::instance()->createTurret(Renderer::instance()->getRenderer(), 1000, 500);
    GameManager::instance()->createWeaponDrop(Renderer::instance()->getRenderer(), 1800, 1700);


    //base = Base();

    GameManager::instance()->addObject(base);
    Point newPoint = base.getSpawnPoint();

    //player = new Player();
    player.setControl(GameManager::instance()->getMarine(playerMarineID));
    player.marine->setPosition(newPoint.first, newPoint.second);

    //camera = Camera(game.window.getWidth(), game.window.getHeight());


    return success;
}

void GameStateMatch::loop() {
    //The frames per second timer
    LTimer fpsTimer;

    //The frames per second cap timer
    //LTimer capTimer;

    //Keeps track of time between steps
    LTimer stepTimer;

    //Start counting frames per second
    unsigned long countedFrames = 0;
    float avgFPS = 0;
    fpsTimer.start();

    // State Loop
    while (play) {
        //Start cap timer
        //capTimer.start();

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

#if 0
        //If frame finished early
        int frameTicks = capTimer.getTicks();
        if ( frameTicks < SCREEN_TICK_PER_FRAME ) {
            //Wait remaining time
            SDL_Delay( SCREEN_TICK_PER_FRAME - frameTicks );
        }
#endif
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
                player.handlePlacementClick(Renderer::instance()->getRenderer());
            }
            break;
        case SDL_KEYDOWN:
            switch( this->event.key.keysym.sym ) {
                case SDLK_ESCAPE:
                    play = false;
                    break;
                case SDLK_b:
                    player.handleTempBarricade(Renderer::instance()->getRenderer());
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

    // Move Camera
    camera.move(player.marine->getX(), player.marine->getY());


}

void GameStateMatch::render() {
    //Only draw when not minimized
    if ( !game.window.isMinimized() ) {

        //Clear screen
        SDL_SetRenderDrawColor( Renderer::instance()->getRenderer(), 0xFF, 0xFF, 0xFF, 0xFF );
        SDL_RenderClear( Renderer::instance()->getRenderer() );

        //Render textures
        for (int i = camera.getX() / TEXTURE_SIZE - 1; ; ++i) {
            if ( i * TEXTURE_SIZE - camera.getX() >= camera.getW()) {
                break;
            }
            for (int j = camera.getY() / TEXTURE_SIZE - 1; ; ++j) {
                if (j * TEXTURE_SIZE - camera.getY() >= camera.getH()) {
                    break;
                }
                Renderer::instance()->render(i * TEXTURE_SIZE - camera.getX(), j * TEXTURE_SIZE -camera.getY(), TEXTURE_SIZE, TEXTURE_SIZE, TEXTURES::BARREN);
            }
        }


        //renders objects in game
        GameManager::instance()->renderObjects(camera.getX(), camera.getY(), camera.getH(), camera.getW());


        SDL_Color textColor = { 0, 0, 0, 255 };

        //Render text
        if ( !frameFPSTextTexture.loadFromRenderedText( frameTimeText.str().c_str(),
                textColor, Renderer::instance()->getRenderer(), frameFont ) ) {
            printf( "Unable to render FPS texture!\n" );
        }

        frameFPSTextTexture.render(Renderer::instance()->getRenderer(),
            (game.window.getWidth() - frameFPSTextTexture.getWidth()), 0);

        //Update screen
        SDL_RenderPresent( Renderer::instance()->getRenderer() );
    }
}

GameStateMatch::~GameStateMatch() {

    // Free texture and font
    delete GameManager::instance();
    frameFPSTextTexture.free();
    TTF_CloseFont(frameFont);
    frameFont = NULL;

}
