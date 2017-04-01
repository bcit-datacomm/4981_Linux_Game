#include <cstdio>
#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "../game/GameStateMatch.h"
#include "../sprites/Renderer.h"
#include "../sprites/SpriteTypes.h"
#include "../basic/LTimer.h"
#include "../view/Window.h"
#include "../log/log.h"
#include "../sprites/VisualEffect.h"
#include "../map/Map.h"
#include "Game.h"



GameStateMatch::GameStateMatch(Game& g, const int gameWidth, const int gameHeight) : GameState(g),
        player(), base(), camera(gameWidth,gameHeight){}

bool GameStateMatch::load() {
    bool success = true;

    const int32_t playerMarineID = GameManager::instance()->createMarine();

    //set the boundary on the map
    // GameManager::instance()->setBoundary(0, 0, MAP_WIDTH, MAP_HEIGHT);
    // Load Map
    Map m("assets/maps/Map4.csv");
    if(m.loadFileData() == 0) {
        logv("file not found");
    }
    m.mapLoadToGame();


    // Create Dummy Entitys
    GameManager::instance()->createMarine(1000, 500);


    //createStores
    GameManager::instance()->createWeaponStore(STORE_X, STORE_Y);

    //createDropPoint
    GameManager::instance()->createDropZone(DROPZONE_X , DROPZONE_Y, DROPZONE_SIZE);

    GameManager::instance()->addObject(base);

    Point newPoint = base.getSpawnPoint();

    //gives the player control of the marine
    player.setControl(GameManager::instance()->getMarine(playerMarineID));
    player.getMarine()->setPosition(newPoint.first, newPoint.second);

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
    int frameTicks;
    unsigned int second = 0;
    fpsTimer.start();

    // State Loop
    while (play) {
        //Start cap timer
        capTimer.start();

        // Process frame
        handle();    // Handle user input
        update(stepTimer.getTicks() / TIME_SECOND); // Update state values
        stepTimer.start(); //Restart step timer
        sync();    // Sync game to server

        render();    // Render game state to window

        if ((stepTimer.getTicks() / TIME_SECOND) > second) {
            GameManager::instance()->createZombieWave(1);
            second += 5;
        }

        //If frame finished early
        if ((frameTicks = capTimer.getTicks()) < SCREEN_TICK_PER_FRAME) {
            //Wait remaining time
            SDL_Delay(SCREEN_TICK_PER_FRAME - frameTicks);
        }
    }
}

void GameStateMatch::sync() {

}

void GameStateMatch::handle() {
    const Uint8 *state = SDL_GetKeyboardState(nullptr); // Keyboard state
    // Handle movement input
    player.handleKeyboardInput(state);
    player.handleMouseUpdate(game.getWindow().getWidth(), game.getWindow().getHeight(), camera.getX(), camera.getY());
    //Handle events on queue
    while (SDL_PollEvent(&event)) {
        game.getWindow().handleEvent(event);
        switch(event.type) {
            case SDL_WINDOWEVENT:
                camera.setViewSize(game.getWindow().getWidth(), game.getWindow().getHeight());
                break;
            case SDL_MOUSEWHEEL:
                player.handleMouseWheelInput(&(event));
                break;
            case SDL_MOUSEBUTTONDOWN:
                if (event.button.button == SDL_BUTTON_RIGHT) {
                    player.handlePlacementClick(Renderer::instance().getRenderer());
                }
                break;
            case SDL_KEYDOWN:
                switch(event.key.keysym.sym) {
                    case SDLK_ESCAPE:
                        play = false;
                        break;
                    case SDLK_b:
                        player.handleTempBarricade(Renderer::instance().getRenderer());
                        break;
                    default:
                        break;
                    }
                    break;
            case SDL_KEYUP:
                switch(event.key.keysym.sym) {
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

void GameStateMatch::update(const float delta) {
    GameManager::instance()->updateCollider();

    // Move player
    GameManager::instance()->updateMarines(delta);
    GameManager::instance()->updateZombies(delta);
    GameManager::instance()->updateTurrets();

    // Move Camera
    camera.move(player.getMarine()->getX(), player.getMarine()->getY());
}

void GameStateMatch::render() {
    //Only draw when not minimized
    if (!game.getWindow().isMinimized()) {

        SDL_RenderClear(Renderer::instance().getRenderer());

        //Render textures
        for (int i = camera.getX() / TEXTURE_SIZE - 1; ; ++i) {

            if (i * TEXTURE_SIZE - camera.getX() >= camera.getW()) {
                break;
            }

            for (int j = camera.getY() / TEXTURE_SIZE - 1; ; ++j) {
                if (j * TEXTURE_SIZE - camera.getY() >= camera.getH()) {
                    break;
                }

                Renderer::instance().render(
                        {i * TEXTURE_SIZE - static_cast<int>(camera.getX()),
                        j * TEXTURE_SIZE - static_cast<int>(camera.getY()),
                        TEXTURE_SIZE, TEXTURE_SIZE}, TEXTURES::BARREN);
            }
        }

        //render the temps before the objects in the game
        VisualEffect::instance().renderPreEntity(camera.getViewport());
        //renders objects in game
        GameManager::instance()->renderObjects(camera.getViewport());
        //render the temps after the object in the game
        VisualEffect::instance().renderPostEntity(camera.getViewport());

        //Update screen
        SDL_RenderPresent(Renderer::instance().getRenderer());
    }
}
