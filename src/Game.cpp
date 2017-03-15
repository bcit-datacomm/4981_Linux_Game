#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <iostream>
#include <stdio.h>
#include <string>
#include "Game.h"
#include "GameStateMatch.h"
#include "GameStateMenu.h"
#include "Window.h"
#include "Player.h"
#include "Base.h"
#include "Level.h"
#include "Camera.h"
#include "Renderer.h"


void Game::run() {
    // End program if stateID is 0 after a end of a loop
    while (stateID > 0) {
        loadState();
        if (state->load()) {
            state->loop();
        }
    }
}

void Game::loadState() {
    printf("Starting ");
    if (state != NULL) {
        delete state;
    }
    // Sets the state by the state ID
    switch(stateID) {
        case 1:
            printf("Menu State\n");
            state = new GameStateMenu(*this, window.getWidth(), window.getHeight());
            break;
        case 2:
            printf("Match State\n");

            state = new GameStateMatch(*this, window.getWidth(), window.getHeight());

            break;
        default:
            break;
    }
     // Reset stateID back to zero to allow states to end program or incase of load failure
    stateID = 0;
}

bool Game::init() {
    //Initialization flag
    bool success = true;

    //Initialize SDL
    if ( SDL_Init( SDL_INIT_VIDEO ) < 0 ) {
        printf( "SDL could not initialize! %s\n", SDL_GetError() );
        success = false;
    } else {
        //Set texture filtering to linear
        if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) ) {
            printf( "Warning: Linear texture filtering not enabled!" );
        }

        //Create window
        window = Window();
        if ( !window.init() ) {
            printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
            success = false;
        } else {
            //Create renderer for window
            Renderer::instance()->setWindow(window.getWindow());
            if ( Renderer::instance()->getRenderer()  == NULL ) {
                printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
                success = false;
            } else {

                //Initialize renderer color
                SDL_SetRenderDrawColor( Renderer::instance()->getRenderer() , 0xFF, 0xFF, 0xFF, 0xFF );

                //Initialize PNG loading
                int imgFlags = IMG_INIT_PNG;
                if ( !( IMG_Init( imgFlags ) & imgFlags ) ) {
                    printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
                    success = false;
                } else {
                    screenSurface = window.getScreenSurface();
                }

                 //Initialize SDL_ttf
                if ( TTF_Init() == -1 ) {
                    printf( "SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError() );
                    success = false;
                }

                //Initialize SDL_mixer
                if ( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 ) {
                    printf( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError() );
                    success = false;
                }

                 screenSurface = window.getScreenSurface();

            }
        }
    }

    return success;
}

bool Game::loadMedia() {
    //Loading success flag
    bool success = true;


    // Load graphics, audio, and fonts here

    return success;
}

void Game::close() {

    if (state != NULL) {
        delete state;
    }

    //Destroy window
    window.free();

    //Quit SDL subsystems
    Mix_Quit();
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();

}
