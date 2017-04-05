#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <cstdio>
#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>

#include "GameStateMatch.h"
#include "../client/NetworkManager.h"
#include "../game/GameStateMatch.h"
#include "../sprites/Renderer.h"
#include "../sprites/SpriteTypes.h"
#include "../view/Window.h"
#include "../log/log.h"
#include "../server/server.h"
#include "../server/servergamestate.h"
#include "../sprites/VisualEffect.h"
#include "../map/Map.h"
#include "Game.h"
#include "../../include/Colors.h"


/**
* Date: Jan. 20, 2017
* Author: Jacob McPhail
* Modified: ---
* Function Interface: GameStateMatch(Game& g,  const int gameWidth, const int gameHeight)
*       g : Pointer to the game object
*       gameWidth : Width of camera view
*       gameHeight : Height of camera view
*               
* Description: 
*       ctor for the match game state.
*/
GameStateMatch::GameStateMatch(Game& g,  const int gameWidth, const int gameHeight) : GameState(g),
        base(), camera(gameWidth,gameHeight), hud(),
        screenRect{0, 0, game.getWindow().getWidth(), game.getWindow().getHeight()}{}

/**
* Date: Jan. 20, 2017
* Author: Jacob McPhail
* Modified: ---
* Function Interface: load() 
* Description: 
*       Loads state resources.
*/
bool GameStateMatch::load() {
#ifndef SERVER
    if (networked) {
        GameManager::instance()->getPlayer().setControl(
                &GameManager::instance()->getMarine(NetworkManager::instance().getPlayerId()).first);
        GameManager::instance()->getPlayer().setId(NetworkManager::instance().getPlayerId());
    } else {
        base.setSrcRect(BASE_SRC_X, BASE_SRC_Y, BASE_SRC_W, BASE_SRC_H);
        GameManager::instance()->addObject(base);
        Point newPoint = base.getSpawnPoint();
        base.setSrcRect(BASE_SRC_X, BASE_SRC_Y, BASE_SRC_W, BASE_SRC_H);

        GameManager::instance()->getPlayer().setControl(
                &GameManager::instance()->getMarine(GameManager::instance()->createMarine()).first);
        GameManager::instance()->getPlayer().getMarine()->setPosition(newPoint.first, newPoint.second);
        GameManager::instance()->getPlayer().getMarine()->setSrcRect(SPRITE_FRONT, SPRITE_FRONT, SPRITE_SIZE_X, SPRITE_SIZE_Y);
    }
#endif

    bool success = true;
    //set the boundary on the map
    // GameManager::instance()->setBoundary(0, 0, MAP_WIDTH, MAP_HEIGHT);

    // Load Map
    Map m("assets/maps/DemoMap.csv");
    if(m.loadFileData() == 0) {
        logv("file not found");
    }
    m.mapLoadToGame();
    GameManager::instance()->setAiMap(m.getAIMap());
    matchManager.setSpawnPoints(m.getZombieSpawn());
    return success;
}

/**
* Date: Jan. 20, 2017
* Author: Jacob McPhail
* Modified: ---
* Function Interface: loop()
* Description: 
*       State loop, processes a frame per each loop.
*/
void GameStateMatch::loop() {
    int startTick = 0;
    int frameTicks = 0;
    // State Loop
    while (play) {
#ifndef SERVER
        // Process frame
        handle(); // Handle user input
#endif
        update((SDL_GetTicks() - startTick) / TICK_SEC); // Update state values
        startTick = SDL_GetTicks();
#ifndef SERVER
        // Sync game to server
        sync();
        // Render game state to window
        render();
#else
        //Server side sync packet sending
        //This will send update packets every frame
        //Currently at 60, so this is fine
        //If frame rate changes, this may need limiting
        sendSyncPacket(sendSocketUDP);
        clearAttackActions();
#endif

        //If frame finished early
        if ((frameTicks = SDL_GetTicks() - startTick) < SCREEN_TICK_PER_FRAME) {
            //Wait remaining time
            SDL_Delay(SCREEN_TICK_PER_FRAME - frameTicks);
        }
    }
}

/**
* Date: Jan. 20, 2017
* Author: Jacob McPhail
* Modified: ---
* Function Interface: sync()
* Description: 
*       Sync game to server.
*/
void GameStateMatch::sync() {

}

void GameStateMatch::updateServ() {

}

/**
 * Function: handle
 *
 * Date: Jan. 20, 2017
 *
 *
 * Designer: Jacob McPhail
 *
 *
 * Programmer: Jacob McPhail
 *
 *
 * Modified by:
 * Jacob Frank (March 25, 2017)
 * Jacob Frank (April, 2017)
 *
 * Interface: handle()
 *
 * Returns: void
 *
 * Notes:
 *      Handles user input.
 * Revisions:
 * JF Mar 25: Added a ScreenRect size adjustment whenever screen size changes (ensures proper hud placement)
 * JF Apr 1: Added set Weapon Inventory slot opacity function to mousewheel scroll and number key events
 */
void GameStateMatch::handle() {
    const Uint8 *state = SDL_GetKeyboardState(nullptr); // Keyboard state
    // Handle movement input if the player has a marine
    if(GameManager::instance()->getPlayer().getMarine()){
        GameManager::instance()->getPlayer().handleKeyboardInput(state);
        GameManager::instance()->getPlayer().handleMouseUpdate(game.getWindow().getWidth(),
                game.getWindow().getHeight(), camera.getX(), camera.getY());
        GameManager::instance()->getPlayer().getMarine()->updateImageDirection(); //Update direction of player
        GameManager::instance()->getPlayer().getMarine()->updateImageWalk(state);  //Update walking animation
    }
    //Handle events on queue
    while (SDL_PollEvent(&event)) {
        game.getWindow().handleEvent(event);
        switch(event.type) {
            case SDL_WINDOWEVENT:
                camera.setViewSize(game.getWindow().getWidth(), game.getWindow().getHeight());
                screenRect = {0, 0, game.getWindow().getWidth(), game.getWindow().getHeight()};
                break;
            case SDL_MOUSEWHEEL:
                if(GameManager::instance()->getPlayer().getMarine()) {
                  GameManager::instance()->getPlayer().handleMouseWheelInput(&event);
                }
                hud.setOpacity(OPAQUE);
                break;
            case SDL_MOUSEBUTTONDOWN:
                if(GameManager::instance()->getPlayer().getMarine()) {
                     if (event.button.button == SDL_BUTTON_RIGHT) {
                        GameManager::instance()->getPlayer().handlePlacementClick(Renderer::instance().getRenderer());
                    } else if (event.button.button == SDL_BUTTON_LEFT) {
                        GameManager::instance()->getPlayer().fireWeapon();
                    }
                }
                break;
            case SDL_KEYDOWN:
                switch(event.key.keysym.sym) {
                    case SDLK_ESCAPE:
                        play = false;
                        break;
                    case SDLK_b:
                        if(GameManager::instance()->getPlayer().getMarine()) {
                                GameManager::instance()->getPlayer().handleTempBarricade(
                                Renderer::instance().getRenderer());
                        }
                        break;
                    case SDLK_1: //Purposeful flow through
                    case SDLK_2:
                    case SDLK_3:
                        hud.setOpacity(OPAQUE);
                        break;
                    case SDLK_k:
                        //k is for kill, sets player marine to a nullptr
                        if (GameManager::instance()->getPlayer().getMarine()) {
                            GameManager::instance()->getPlayer().getMarine()->setHealth(0);
                        }
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

/**
* Date: Jan. 20, 2017
* Author: Jacob McPhail
* Modified: ---
* Function Interface: update(const float delta)
*       delta : Delta time of the fps rate.       
*
* Description: 
* 
*/
void GameStateMatch::update(const float delta) {
    GameManager::instance()->updateCollider();
#ifndef SERVER
    // Move player
    if (networked) {
        if (GameManager::instance()->getPlayer().hasChangedCourse()
                || GameManager::instance()->getPlayer().hasChangedAngle()) {
            GameManager::instance()->getPlayer().sendServMoveAction();
        }
        GameManager::instance()->getPlayer().getMarine()->move(
                GameManager::instance()->getPlayer().getMarine()->getDX() * delta,
                GameManager::instance()->getPlayer().getMarine()->getDY() * delta,
                GameManager::instance()->getCollisionHandler());
    }

#endif
    GameManager::instance()->updateMarines(delta);
    GameManager::instance()->updateZombies(delta);
    GameManager::instance()->updateTurrets();
    GameManager::instance()->getPlayer().checkMarineState();
    matchManager.checkMatchState();

#ifndef SERVER
    // Move Camera
    if(GameManager::instance()->getPlayer().getMarine()){
        camera.move(GameManager::instance()->getPlayer().getMarine()->getX(), GameManager::instance()->getPlayer().getMarine()->getY());
    }
    if (GameManager::instance()->getPlayer().checkMarineState()) {
        GameManager::instance()->getPlayer().respawn(base.getSpawnPoint());
    }
#endif
}

/**
 * Function: render
 *
 * Date: Jan. 20, 2017
 *
 *
 * Designer: Jacob McPhail
 *
 *
 * Programmer: Jacob McPhail
 *
 *
 * Modified by:
 * Jacob Frank (March 28 - April 1, 2017)
 *
 * Interface: render()
 *
 * Returns: void
 *
 * Notes:
 *      Renders game objects to window.
 * Revisions:
 * JF Mar 25 - April 1: Added rendering functions to render the HUD overtop of the game
 */
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
        if (GameManager::instance()->getPlayer().getMarine()) {
            //Render the healthbar's foreground to the screen
            //(displays how much player health is left)
            hud.renderHealthBar(screenRect, GameManager::instance()->getPlayer(), camera);
            //Reder the ammo clip foreground to the screen
            //(displays how much ammo is left in the players weapon clip)
            hud.renderClip(screenRect, GameManager::instance()->getPlayer());
           //Render the healthbar's foreground to the screen
           //(displays how much player health is left)
           //hud.renderHealthBar(screenRect, GameManager::instance()->getPlayer(), camera);
          //Reder the ammo clip foreground to the screen
           //(displays how much ammo is left in the players weapon clip)
           //hud.renderClip(screenRect, GameManager::instance()->getPlayer());


            //Render the equipped weapon slot
            hud.renderEquippedWeaponSlot(screenRect, GameManager::instance()->getPlayer());

            //Reder the Weapon slots to the screen
            hud.renderWeaponSlots(screenRect, GameManager::instance()->getPlayer());

            //Render the consumable slot if the player has any available
            //Currently only a single consumable item exits (the Medkit)
            if (GameManager::instance()->getPlayer().getMarine()->inventory.getMedkit()) {
                hud.renderConsumable(screenRect, GameManager::instance()->getPlayer());
            }
        }
        //Update screen
        SDL_RenderPresent(Renderer::instance().getRenderer());
    }
}
