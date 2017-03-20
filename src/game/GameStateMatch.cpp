#include <stdio.h>
#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "GameStateMatch.h"
#include "../client/NetworkManager.h"
#include "../game/GameStateMatch.h"
#include "../sprites/Renderer.h"
#include "../sprites/SpriteTypes.h"
#include "../basic/LTimer.h"
#include "../view/Window.h"
#include "../log/log.h"
#include "../server/server.h"
#include "../server/servergamestate.h"

GameStateMatch::GameStateMatch(Game& g,  int gameWidth, int gameHeight) : GameState(g), player(),
    base(), camera(gameWidth,gameHeight) {

}

bool GameStateMatch::load() {
    if (networked) {
        player.setControl(GameManager::instance()->getMarine(NetworkManager::instance().getPlayerId()));
        player.setId(NetworkManager::instance().getPlayerId());
    } else {
        Point newPoint = base.getSpawnPoint();
        player.setControl(GameManager::instance()->getMarine(GameManager::instance()->createMarine()));
        player.marine->setPosition(newPoint.first, newPoint.second);

        // Create Dummy Entitys
        GameManager::instance()->createMarine(100, 100);
        GameManager::instance()->createZombie(800, 800);
        GameManager::instance()->createTurret(1000, 500);
        GameManager::instance()->createWeaponDrop(1800, 1700);
    }

    bool success = true;
    //const int32_t playerMarineID = GameManager::instance()->createMarine();

    //set the boundary on the map
    GameManager::instance()->setBoundary(-1000, -1000, 3000, 3000);

    //creates the base
    GameManager::instance()->addObject(base);
    // Create Dummy Entitys
    //GameManager::instance()->createMarine(100, 100);
    GameManager::instance()->createZombie(800, 800);
    GameManager::instance()->createTurret(1000, 500);
    GameManager::instance()->createWeaponDrop(1800, 1700);

    //creates the base
    GameManager::instance()->addObject(base);
    //Point newPoint = base.getSpawnPoint();

    //gives the player control of the marine
    //player.setControl(GameManager::instance()->getMarine(playerMarineID));
    //player.marine->setPosition(newPoint.first, newPoint.second);

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
    int frameTicks;
    unsigned int second = 0;
#ifndef SERVER
    float avgFPS = 0;
#endif
    fpsTimer.start();

    // State Loop
    while (play) {
        //Start cap timer
        capTimer.start();
#ifndef SERVER
        //Calculate and correct fps
        avgFPS = countedFrames / (fpsTimer.getTicks() / TIME_SECOND);

        //Set FPS text to be rendered
        frameTimeText.str("");
        frameTimeText << std::fixed << std::setprecision(0) << "FPS: " << avgFPS;
#endif
        // Process frame
        handle();    // Handle user input

        update(stepTimer.getTicks() / 1000.f); // Update state values
        if (networked && stepTimer.getTicks() % (SCREEN_TICK_PER_FRAME * (1/30)+1) == 0)
            updateServ();

        stepTimer.start(); //Restart step timer
#ifndef SERVER
        sync();    // Sync game to server
        render();    // Render game state to window
#else
        if (countedFrames % 2 == 0) {
            //Server side sync packet sending
            genOutputPacket();
            sendSyncPacket(sendSocketUDP);
        }
#endif

        ++countedFrames;

        if(fpsTimer.getTicks() / TIME_SECOND > second) {
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

void GameStateMatch::updateServ() {
    MoveAction moveAction = player.getMoveAction();
    ClientMessage clientMessage;
    clientMessage.id = static_cast<int32_t>(UDPHeaders::WALK);
    clientMessage.data.ma = moveAction;
    NetworkManager::instance().writeUDPSocket((char *)&clientMessage, sizeof(ClientMessage));
}

void GameStateMatch::sync() {

}

void GameStateMatch::handle() {
    const Uint8 *state = SDL_GetKeyboardState(nullptr); // Keyboard state
    // Handle movement input
    player.handleKeyboardInput(state);
    player.handleMouseUpdate(game.window, camera.getX(), camera.getY());
    //Handle events on queue
    while (SDL_PollEvent(&event)) {
        game.window.handleEvent(event);
           switch(event.type) {
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
            switch(event.key.keysym.sym) {
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
    if (networked) {
        player.marine->move(player.marine->getDX() * delta, player.marine->getDY() * delta,
            GameManager::instance()->getCollisionHandler());
    } else {
        GameManager::instance()->updateMarines(delta);
        GameManager::instance()->updateZombies(delta);
    }
    // Move Camera
    camera.move(player.marine->getX(), player.marine->getY());
}

void GameStateMatch::render() {
    //Only draw when not minimized
    if (!game.window.isMinimized()) {

        SDL_RenderClear(Renderer::instance()->getRenderer());

        //Render textures
        for (int i = camera.getX() / TEXTURE_SIZE - 1; ; ++i) {

            if (i * TEXTURE_SIZE - camera.getX() >= camera.getW()) {
                break;
            }

            for (int j = camera.getY() / TEXTURE_SIZE - 1; ; ++j) {
                if (j * TEXTURE_SIZE - camera.getY() >= camera.getH()) {
                    break;
                }

                Renderer::instance()->render(
                        {i * TEXTURE_SIZE - camera.getX(), j * TEXTURE_SIZE -camera.getY(), TEXTURE_SIZE, TEXTURE_SIZE},
                        TEXTURES::BARREN);
            }
        }

        //renders objects in game
        GameManager::instance()->renderObjects(camera.getViewport());

        //Update screen
        SDL_RenderPresent(Renderer::instance()->getRenderer());
    }
}
