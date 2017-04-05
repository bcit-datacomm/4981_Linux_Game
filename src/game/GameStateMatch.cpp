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
#include "../view/Window.h"
#include "../log/log.h"
#include "../sprites/VisualEffect.h"
#include "../map/Map.h"
#include "Game.h"
#include "../../include/Colors.h"


GameStateMatch::GameStateMatch(Game& g,  const int gameWidth, const int gameHeight) : GameState(g),
        player(), base(), camera(gameWidth,gameHeight), hud(),
        screenRect{0, 0, game.getWindow().getWidth(), game.getWindow().getHeight()}{}

bool GameStateMatch::load() {
    bool success = true;

    const int32_t playerMarineID = GameManager::instance()->createMarine();

    // Load Map
    Map m("assets/maps/Map4.csv");
    if(m.loadFileData() == 0) {
        logv("file not found");
    }
    m.mapLoadToGame();
    GameManager::instance()->setAiMap(m.getAIMap());

    // Create Dummy Entitys
    GameManager::instance()->createMarine(850, 500);
    //createStores
    GameManager::instance()->createWeaponStore(STORE_X, STORE_Y);

    //createDropPoint
    GameManager::instance()->createDropZone(DROPZONE_X , DROPZONE_Y, DROPZONE_SIZE);
    Rifle w(GameManager::instance()->generateID());
    ShotGun w2(GameManager::instance()->generateID());
    GameManager::instance()->addWeapon(std::dynamic_pointer_cast<Weapon>(std::make_shared<Rifle>(w)));
    GameManager::instance()->addWeapon(std::dynamic_pointer_cast<Weapon>(std::make_shared<ShotGun>(w2)));
    GameManager::instance()->createWeaponDrop(1200, 500, w.getID());
    GameManager::instance()->createWeaponDrop(1200, 300, w2.getID());

    base.setSrcRect(BASE_SRC_X, BASE_SRC_Y, BASE_SRC_W, BASE_SRC_H);
    GameManager::instance()->addObject(base);

    Point newPoint = base.getSpawnPoint();

    //gives the player control of the marine
    player.setControl(&GameManager::instance()->getMarine(playerMarineID));
    player.getMarine()->setPosition(newPoint.first, newPoint.second);
    player.getMarine()->setSrcRect(SPRITE_FRONT, SPRITE_FRONT, SPRITE_SIZE_X, SPRITE_SIZE_Y);

    matchManager.setSpawnPoints(m.getZombieSpawn());

    return success;
}

void GameStateMatch::loop() {
    int startTick = 0;
    int frameTicks = 0;
    // State Loop
    while (play) {
        handle(); // Handle user input
        update((SDL_GetTicks() - startTick) / TICK_SEC); // Update state values

        startTick = SDL_GetTicks();
        // Sync game to server
        sync();
        // Render game state to window
        render();

        //If frame finished early
        if ((frameTicks = SDL_GetTicks() - startTick) < SCREEN_TICK_PER_FRAME) {
            //Wait remaining time
            SDL_Delay(SCREEN_TICK_PER_FRAME - frameTicks);
        }
    }
}

void GameStateMatch::sync() {

}

/**
 * Function: handle
 *
 * Date:
 *
 *
 * Designer:
 *
 *
 * Programmer:
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
 *
 * Revisions:
 * JF Mar 25: Added a ScreenRect size adjustment whenever screen size changes (ensures proper hud placement)
 * JF Apr 1: Added set Weapon Inventory slot opacity function to mousewheel scroll and number key events
 */
void GameStateMatch::handle() {
    const Uint8 *state = SDL_GetKeyboardState(nullptr); // Keyboard state
    // Handle movement input if the player has a marine
    if(player.getMarine() != nullptr){
        player.handleKeyboardInput(state);
        player.handleMouseUpdate(game.getWindow().getWidth(), game.getWindow().getHeight(), camera.getX(), camera.getY());
        player.getMarine()->updateImageDirection(); //Update direction of player
        player.getMarine()->updateImageWalk(state);  //Update walking animation
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
                player.handleMouseWheelInput(&(event));
                hud.setOpacity(OPAQUE);
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
                    case SDLK_1: //Purposeful flow through
                    case SDLK_2:
                    case SDLK_3:
                        hud.setOpacity(OPAQUE);
                        break;
                    case SDLK_k:
                        //k is for kill, sets player marine to a nullptr
                        GameManager::instance()->deleteMarine(player.getMarine()->getId());
                        player.setControl(nullptr);
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
    if(player.getMarine()){
        camera.move(player.getMarine()->getX(), player.getMarine()->getY());
    }
    player.checkMarineState();
    matchManager.checkMatchState();
}

/**
 * Function: render
 *
 * Date:
 *
 *
 * Designer:
 *
 *
 * Programmer:
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
 *
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

        if (player.getMarine()) {
            //Render the healthbar's foreground to the screen
            //(displays how much player health is left)
            hud.renderHealthBar(screenRect, player, camera);

            //Reder the ammo clip foreground to the screen
            //(displays how much ammo is left in the players weapon clip)
            hud.renderClip(screenRect, player);


            //Render the equipped weapon slot
            hud.renderEquippedWeaponSlot(screenRect, player);

            //Reder the Weapon slots to the screen
            hud.renderWeaponSlots(screenRect, player);

            //Render the consumable slot if the player has any available
            //Currently only a single consumable item exits (the Medkit)
            if (player.getMarine()->inventory.getMedkit()) {
                hud.renderConsumable(screenRect, player);
            }
        }
        //Update screen
        SDL_RenderPresent(Renderer::instance().getRenderer());
    }
}
