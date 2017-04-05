#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <iostream>
#include <stdio.h>
#include <string>

#include "../server/server.h"
#include "../game/Game.h"
#include "../game/GameStateMatch.h"
#include "../game/GameStateMenu.h"
#include "../view/Window.h"
#include "../player/Player.h"
#include "../buildings/Base.h"
#include "../game/Level.h"
#include "../sprites/Renderer.h"
#include "../view/Camera.h"
#include "../log/log.h"

/**
* Date: Jan. 20, 2017
* Author: Jacob McPhail
* Modified: ---
* Function Interface: ~Game()
* Description: 
*   dctor for game, quits the SDL subsystems.
*/
Game::~Game() {
    state.reset();

    //Quit SDL subsystems
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}

/**
* Date: Jan. 20, 2017
* Author: Jacob McPhail
* Modified: ---
* Function Interface: run()
* Description: 
*   Main while loop of the game, quits on no stateID to load.
*/
void Game::run() {
    // End program if stateID is 0 after a end of a loop
    while (stateID > 0) {
        logv("State ID: %d\n", stateID);
        loadState();
        if (state->load()) {
#ifdef SERVER
            isGameRunning.store(true, std::memory_order_relaxed);
#endif
            state->loop();
        }
    }
}

/**
* Date: Jan. 20, 2017
* Author: Jacob McPhail
* Modified: ---
* Function Interface: 
* Description: 
*   Loads a state to be used.
*/
void Game::loadState() {
#ifdef SERVER
    state = std::make_unique<GameStateMatch>(*this, window.getWidth(), window.getHeight());
    stateID = 0;
#else
    logv("Starting ");
    state.reset();
    // Sets the state by the state ID
    switch(stateID) {
        case 1:
            logv("Menu State\n");
            state = std::make_unique<GameStateMenu>(*this);
            break;
        case 2:
            logv("Match State\n");
            state = std::make_unique<GameStateMatch>(*this, window.getWidth(), window.getHeight());
            break;
        default:
            break;
    }
     // Reset stateID back to zero to allow states to end program or incase of load failure
    stateID = 0;
#endif
}

/**
* Date: Jan. 20, 2017
* Author: Jacob McPhail
* Modified: ---
* Function Interface: init()
* Description: 
*   Setups the SDL components, renderer, and program window for the game to use.
*/
bool Game::init() {
    //Initialization flag
    bool success = true;

    //Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        logv("SDL could not initialize! %s\n", SDL_GetError());
        success = false;
    } else {
        //Set texture filtering to linear
        if(!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) {
            logv("Warning: Linear texture filtering not enabled!");
        }

        //Create window
        window = Window();
        if (!window.init()) {
            logv("Window could not be created! SDL Error: %s\n", SDL_GetError());
            success = false;
        } else {
            //Create renderer for window
            Renderer::instance().setWindow(window.getWindow());
            if (Renderer::instance().getRenderer()  == nullptr) {
                logv("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
                success = false;
            } else {
                //Initialize renderer color
                SDL_SetRenderDrawColor(Renderer::instance().getRenderer() , 0xFF, 0xFF, 0xFF, 0xFF);

                //Initialize PNG loading
                int imgFlags = IMG_INIT_PNG;
                if (!(IMG_Init(imgFlags) & imgFlags)) {
                    logv("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
                    success = false;
                } else {
                    screenSurface = window.getScreenSurface();
                }

                 //Initialize SDL_ttf
                if (TTF_Init() == -1) {
                    logv("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
                    success = false;
                }

#ifndef SERVER
                //Initialize SDL_mixer
                if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
                    logv("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
                    success = false;
                }
#endif
            }
        }
    }
    return success;
}

/**
* Date: Jan. 20, 2017
* Author: Jacob McPhail
* Modified: ---
* Function Interface: loadMedia()
* Description: 
*   Loads media (audio, textures) to be used for the game.
*/
bool Game::loadMedia() {
    Renderer::instance().loadSprites();
    return true;
}

/**
* Date: Jan. 20, 2017
* Author: Jacob McPhail
* Modified: ---
* Function Interface: close()
* Description: 
*   Kills the SDL components.
*/
void Game::close() {
    //Quit SDL subsystems
#ifndef SERVER
    Mix_Quit();
#endif
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}
