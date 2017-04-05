#include <stdio.h>
#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <unistd.h>

#include "GameStateMenu.h"
#include "Game.h"
#include "../view/Window.h"
#include <unistd.h>
#include "../client/NetworkManager.h"
#include "../sprites/Renderer.h"
#include "../log/log.h"
#include "../sprites/SpriteTypes.h"
#include "../sprites/Textomagic.h"


/**
 * Function: GameStateMenu ctor
 *
 * Date:
 * JF: March 8, 2017: added member initilizers
 *
 * Designer:
 * Jacob Frank
 *
 * Programmer:
 * Jacob Frank
 *
 * Modified by:
 * Jacob McPhail (Feburay 2, 2017) - Init Creation
 * Michael Goll (March 16, 2017)
 * Jacob Frank (March 26, 2017)
 *
 * Interface: GameStateMenu(Game& g, int gameWidth, int gameHeight)
 *                           Game& g: The instance of the game which the display window is tied to
 *                           int gameWidth: Width of the game window
 *                           int gameHeight: Height of the game window
 *
 * Notes:
 * GameStateMenu ctor which initializes all member variables for use throughout class
 *
 * Revisions:
 * Removed excess initializations. (Michael Goll / March 16, 2017)
 * JF Mar 26: Re-added non-excessive initializations
 * Isaac Morneau, March 29, 2017 Re-removed the initializations that were in fact excessive
 *      then refactored the menu system to operate more logically and cleanly
*/
GameStateMenu::GameStateMenu(Game& g):GameState(g),
        screenRect{0, 0, g.getWindow().getWidth(), g.getWindow().getHeight()},
        hostRect{
            static_cast<int>(screenRect.w * HOST_X_RAT),
            static_cast<int>(screenRect.h * HOST_Y_RAT),
            static_cast<int>(screenRect.w * HOST_W_RAT),
            static_cast<int>(screenRect.h * HOST_H_RAT)},
        userRect{
            static_cast<int>(screenRect.w * USER_X_RAT),
            static_cast<int>(hostRect.y + hostRect.h + USER_Y_PAD),
            static_cast<int>(screenRect.w * USER_W_RAT),
            static_cast<int>(screenRect.h * USER_H_RAT)},
        joinRect{
            static_cast<int>(screenRect.w * JOIN_X_RAT),
            static_cast<int>(userRect.y + userRect.h + JOIN_Y_PAD),
            static_cast<int>(screenRect.w * JOIN_W_RAT),
            static_cast<int>(screenRect.h * JOIN_H_RAT)},
        joinSelected(false), hostIPSelected(false), usernameSelected(false),
        hostInput(HOST_DEFAULT), userInput(USER_DEFAULT),
        hostMagic(hostRect, Renderer::instance().loadFont("assets/fonts/SEGOEUISL.ttf", FONT_SIZE), maxLength, HOST_DEFAULT),
        userMagic(userRect, Renderer::instance().loadFont("assets/fonts/SEGOEUISL.ttf", FONT_SIZE), maxLength, USER_DEFAULT),
        joinMagic(joinRect,Renderer::instance().loadFont("assets/fonts/Overdrive Sunset.otf", FONT_SIZE), JOIN_LEN, "join") {

}


/**
 * Function: load
 *
 * Date:
 * JF: February 8, 2017: added handler for window resizing event
 *
 * Designer:
 * Jacob Frank
 *
 * Programmer:
 * Jacob Frank
 *
 * Modified by:
 * Jacob McPhail (Feburay 2, 2017) - Init Creation
 * Michael Goll (March 16, 2017)
 * Jacob Frank (March 26. 2017)
 *
 * Interface: load()
 *
 * Returns: true if the level, camera, and all font textures were loaded without error
 *
 * Notes:
 * Function loads the initial state of the window
 * Opens all fonts to be used and the title screen background
 *
 * Revisions:
 * Loads all text files and needed text textures once and stores them in Renderer instance's sprite array.
 * (Michael Goll / March 16, 2017)
 * JF Mar 26: Changed the font texture and added Textures for active fonts
 * Isaac Morneau:
 *      off loaded loading into constructor and Textomagic objects
 */
bool GameStateMenu::load() {
    return true;
}


/**
 * Function: loop
 *
 * Date:
 * JF: February 8, 2017
 *
 * Designer:
 * Jacob Frank
 *
 * Programmer:
 * Jacob Frank
 *
 * Modified by:
 * Jacob McPhail (Feburay 2, 2017) - Init Creation
 *
 * Interface: loop()
 *
 * Returns: void
 *
 * Notes:
 * Function acts as main loop for the menu game state
 * Listens for events and renders all assets to the screen
 */
void GameStateMenu::loop() {
    // State Loop
    while (play) {
        if(networked) {
            const NetworkState netState = NetworkManager::instance().getNetworkState();
            if (netState == NetworkState::FAILED_TO_CONNECT) {

                NetworkManager::instance().reset();
            } else if (netState >= NetworkState::GAME_STARTED) {
                game.setStateID(2);
                play = false;
            }
        }

        handle(); // Handle user input
        render(); // Render game state to window
    }
}

/**
 * Function: sync
 *
 * Date: Feb 2, 2017
 *
 * Designer: Jacob McPhail
 *
 * Programmer: Jacob McPhail
 *
 * Interface: sync()
 *
 * Returns: void
 *
 * Notes:
 * Function currently empty
 * Designed to be used by Game Logic team for connecting to the server
 */
void GameStateMenu::sync() {

}

/**
 * Function: handle
 *
 * Date:
 * JF: February 8, 2O17: added handler for window resizing event
 *
 * Designer:
 * Jacob Frank
 *
 * Programmer:
 * Jacob Frank
 *
 * Modified by:
 * Jacob McPhail (Feburay 2, 2017) - Init Creation
 * Jacob Frank (March 28, 2017)
 *
 * Interface: handle()
 *
 * Returns: void
 *
 * Notes:
 * Function handles events occuring within the window to control
 * functionality of the main menu
 *
 * Revisions:
 * JF Mar 28: Removed the Options menu item as it was removed from the scope of the game
 * JF Mar 28: Re-added logic for typing text into the text edit boxes that was removed during the great refactoring
 * JF Mar 28: Re-added logic Highlighting and Clicking a menu option that was removed during the great refactoring
 * Isaac Morneau, March 29, 2017 Fixed highlighting, clicking, and typing to work with the improved renderer that now
 *      makes sense because of the god send that was the great refactoring.
 */
void GameStateMenu::handle() {
    int x, y;

    SDL_Keycode keyCode;

    //Handle events on queue
    SDL_WaitEvent(&event);
    game.getWindow().handleEvent(event);

    switch (event.type) {

        case SDL_MOUSEBUTTONDOWN:
            x = event.button.x;
            y = event.button.y;

            if (joinSelected) {
                if (networked) {
                    NetworkManager::instance().run(hostInput, userInput);
                } else {
                    game.setStateID(2); //changes the state to tell the Game.cpp loop to start the actual game
                    play = false;
                }
                break;
            }

            //Checks the mouse position when the mouse click occurred and sets the host Ip textbox
            //to it's acive state when the mouse button down event occurred within the textbox's bounds
            if (x >= hostRect.x && x <= hostRect.x + hostRect.w &&
                    y >= hostRect.y && y <= hostRect.y + hostRect.h) {
                hostIPSelected = true;  //Activate the button

                //If default string is detected, delete it
                if (hostInput.compare(HOST_DEFAULT) == 0) {
                    hostInput = "";
                }
            } else {
                hostIPSelected = false; //deactivate the button

                //Reset textbox text to default text, if textbox is empty
                if (hostInput.empty()) {
                    hostInput = HOST_DEFAULT;
                }
            }

            //Checks the mouse position when the mouse click occurred and sets the username textbox
            //to it's acive state when the mouse button down event occurred within the textbox's bounds
            if (x >= userRect.x && x <= userRect.x + userRect.w &&
                    y >= userRect.y && y <= userRect.y + userRect.h) {
                usernameSelected = true;  //Activate the button

                //If default string is detected, delete it
                if (userInput.compare(USER_DEFAULT) == 0) {
                    userInput = "";
                }
            } else {
                usernameSelected = false; //deactivate the button

                //Reset textbox text to default text, if textbox is empty
                if (userInput.empty()) {
                    userInput = USER_DEFAULT;
                }
            }
            break;

        case SDL_KEYDOWN:
            keyCode = event.key.keysym.sym;

            if (keyCode == SDLK_ESCAPE) {
                play = false;
                break;
            }

            //Handling backspace
            if (hostIPSelected) {
                if (keyCode == SDLK_BACKSPACE && hostInput.length() > 0) {
                    hostInput.pop_back();
                }
            } else if (usernameSelected) {
                if (keyCode == SDLK_BACKSPACE && userInput.length() > 0) {
                    userInput.pop_back();
                }
            }

            if (hostIPSelected || usernameSelected) {
                //Handle copy
                if (keyCode == SDLK_c && SDL_GetModState() & KMOD_CTRL) {
                    if (hostIPSelected) {
                        SDL_SetClipboardText(hostInput.c_str());
                    } else if (usernameSelected) {
                        SDL_SetClipboardText(userInput.c_str());
                    }
                }
                //Handle paste
                else if (keyCode == SDLK_v && SDL_GetModState() & KMOD_CTRL) {
                    if (hostIPSelected) {
                        hostInput += SDL_GetClipboardText();
                    } else if (usernameSelected) {
                        userInput += SDL_GetClipboardText();
                    }
                }
            }
            break;

        case SDL_TEXTINPUT:
            if (hostIPSelected) {
                //Update the string int displayed in the host IP textbox
                if (hostInput.length() < maxLength) {
                    hostInput += event.text.text;
                }
            } else if (usernameSelected) {
                //Update the string int displayed in the username textbox
                if (userInput.length() < maxLength) {
                    userInput += event.text.text;
                }
            }
            break;

        case SDL_KEYUP: //Do nothing on key release
            break;

        case SDL_MOUSEMOTION:
            x = event.motion.x;
            y = event.motion.y;

            //Checks the mouse position and sets the menu option Join
            //to it's acive state when the mouse hovers above the Join text
            joinSelected = (x >= joinRect.x && x <= joinRect.x + joinRect.w &&
                y >= joinRect.y && y <= joinRect.y + joinRect.h);
            break;

        case SDL_WINDOWEVENT:
            switch (event.window.event) {
                case SDL_WINDOWEVENT_RESIZED:
                    //Re-render with the new size
                    screenRect = {0, 0, event.window.data1, event.window.data2};
                    //Position all screen elements in the window
                    positionElements();
                    break;
            }
            break;

        case SDL_QUIT:
            play = false;
            game.setStateID(0);
            break;

        default:
            break;
    }
}

/**
 * Function: update
 *
 * Date:
 * JF: February 8, 2017: Moved positional data into own method
 *
 * Designer:
 * Jacob Frank
 *
 * Programmer:
 * Jacob Frank
 *
 * Modified by:
 * Jacob McPhail (Feburay 2, 2017) - Init Creation
 * Jacob Frank (Febuary 8, 2017)
 *
 * Interface: update(const float& delta)
 *
 * Returns: void
 *
 * Notes:
 * Function positions all screen elements in the window
 *
 * Revisions:
 * JF Feb 8: Moved positional data into own method positionElements()
 */
void GameStateMenu::update(const float delta) {

}

/**
 * Function: positionElements
 *
 * Date:
 * JF: March 6, 2017: Moved positional data into own method
 *
 * Designer:
 * Jacob Frank
 *
 * Programmer:
 * Jacob Frank
 *
 * Modified by:
 * Michael Goll (March 16, 2017)
 * Jacob Frank   (March 26, 2017)
 * Jacob Frank   (March 28, 2017)
 *
 * Interface: positionElements()
 *
 * Returns: void
 *
 * Notes:
 * Function positions all screen elements in the window
 *
 * Revisions:
 * Renders text in SDL_Rect structures instead of LTextures. (Michael Goll / March 16, 2017)
 * JF Mar 26, 2017: Fixed positional data from restructuring not setting assets in correct position
 * JF Mar 28: Removed the Options menu item as it was removed from the scope of the game
 * JF Mar 28: Added chat box below Join Button for the displaying of chat and server information
 * Isaac Morneau, March 29, 2017: Replaced all the incremental calculations with ratios
 */
void GameStateMenu::positionElements() {
    screenRect = {0, 0, game.getWindow().getWidth(), game.getWindow().getHeight()};

    hostRect = {static_cast<int>(screenRect.w * HOST_X_RAT),
        static_cast<int>(screenRect.h * HOST_Y_RAT),
        static_cast<int>(screenRect.w * HOST_W_RAT),
        static_cast<int>(screenRect.h * HOST_H_RAT)};

    userRect = {static_cast<int>(screenRect.w * USER_X_RAT),
        static_cast<int>(hostRect.h + hostRect.y + USER_Y_PAD),
        static_cast<int>(screenRect.w * USER_W_RAT),
        static_cast<int>(screenRect.h * USER_H_RAT)};

    joinRect = { static_cast<int>(screenRect.w * JOIN_X_RAT),
        static_cast<int>(userRect.h + userRect.y + JOIN_Y_PAD),
        static_cast<int>(screenRect.w * JOIN_W_RAT),
        static_cast<int>(screenRect.h * JOIN_H_RAT) };

    hostMagic.setRect(hostRect);
    userMagic.setRect(userRect);
    joinMagic.setRect(joinRect);
}

/**
 * Function: render
 *
 * Date:
 * JF: February 20, 2017: created function
 * JF: March 7-8, 2017: Moved functionality for rendering to wrapper function
 *
 * Designer:
 * Jacob Frank
 *
 * Programmer:
 * Jacob Frank
 *
 * Modified by:
 * Jacob McPhail (Feburay 2, 2017) - Init Creation
 * Michael Goll (March 16, 2017)
 * Jacob Frank   (March 28, 2017)
 *
 * Interface: render()
 *
 * Returns: void
 *
 * Notes:
 * Function renders all assets to the screen
 * Changes the color of any assets that are selected
 * Calls helper function to position elements in the window
 *
 * Revisions:
 * Now renders solely with the Renderer instance. (Michael Goll / March 16, 2017)
 * JF Mar 28: Removed the Options menu item as it was removed from the scope of the game
 * Isaac Morneau, March 29, 2017: Replaced raw rendering with the wrapped text rendering of
 *      the textomagic objects.
 */
void GameStateMenu::render() {
    //Only draw when not minimized
    if (game.getWindow().isMinimized()) {
        return;
    }

    //Clear screen
    SDL_RenderClear(Renderer::instance().getRenderer());

    //render the splash screen
    Renderer::instance().render(screenRect, TEXTURES::MAIN);

    //Join and Options text
    //Change the color of the text when active
    if (joinSelected) {
        joinMagic.setColor(GREEN);
    } else {
        joinMagic.setColor(RED);
    }

    //Host IP and Username textboxes
    //Change the color of the textbox when active
    //Used so User knows when textbox is can accept input
    if (hostIPSelected) {
        Renderer::instance().render(hostRect, TEXTURES::TEXTBOX_ACTIVE);
        hostMagic.setColor(RED);
    } else {
        Renderer::instance().render(hostRect, TEXTURES::TEXTBOX);
        hostMagic.setColor(BLACK);
    }

    if (usernameSelected) {
        Renderer::instance().render(userRect, TEXTURES::TEXTBOX_ACTIVE);
        userMagic.setColor(RED);
    } else {
        Renderer::instance().render(userRect, TEXTURES::TEXTBOX);
        userMagic.setColor(BLACK);
    }

    userMagic.setText(userInput);
    hostMagic.setText(hostInput);

    hostMagic.render();
    userMagic.render();
    joinMagic.render();

    //Update screen
    SDL_RenderPresent(Renderer::instance().getRenderer());
}
