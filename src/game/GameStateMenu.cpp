#include <stdio.h>
#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <unistd.h>

#include "../game/GameStateMenu.h"
#include "../basic/LTimer.h"
#include "../view/Window.h"
#include "../sprites/Renderer.h"
#include "../log/log.h"
#include "../sprites/SpriteTypes.h"
#include "../../include/Colors.h"
#include "../sprites/Textomagic.h"


static constexpr SDL_Color BLACK = {0, 0, 0, 255};
static constexpr SDL_Color RED = {143, 63, 61, 255};

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

*/
GameStateMenu::GameStateMenu(Game& g):GameState(g),
        screenRect{0, 0, game.window.getWidth(), game.window.getHeight()},
        hostRect{
            static_cast<int>(screenRect.w * 4 / 12), 
            static_cast<int>(screenRect.h * 1 / 2), 
            static_cast<int>(screenRect.w * 1 / 3), 
            static_cast<int>(screenRect.h * 1 / 14)},
        userRect{
            static_cast<int>(screenRect.w * 4 / 12), 
            static_cast<int>(screenRect.h * 9 / 14), 
            static_cast<int>(screenRect.w * 1 / 3), 
            static_cast<int>(screenRect.h * 1 / 14)},
        joinRect{
            static_cast<int>(screenRect.w * 5 / 12), 
            static_cast<int>(screenRect.h * 11 / 14), 
            static_cast<int>(screenRect.w * 1 / 6), 
            static_cast<int>(screenRect.h * 1 / 14)},
        joinSelected(false), hostIPSelected(false), usernameSelected(false),
        hostInput(HOST_DEFAULT), userInput(USER_DEFAULT),
        hostMagic(hostRect, Renderer::instance().loadFont("assets/fonts/SEGOEUISL.ttf", FONT_SIZE), maxLength, HOST_DEFAULT),
        userMagic(userRect, Renderer::instance().loadFont("assets/fonts/SEGOEUISL.ttf", FONT_SIZE), maxLength, USER_DEFAULT),
        joinMagic(joinRect,Renderer::instance().loadFont("assets/fonts/Overdrive Sunset.otf", 110), 4, "  join") {

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
        handle(); // Handle user input
        render(); // Render game state to window
    }
}

/**
 * Function: sync
 *
 * Date:
 *
 * Designer:
 *
 * Programmer:
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
 */
void GameStateMenu::handle() {
    int x, y;

    SDL_Keycode keyCode;

    //Handle events on queue
    SDL_WaitEvent(&event);
    game.window.handleEvent(event);

    switch (event.type) {

        case SDL_MOUSEBUTTONDOWN:
            x = event.button.x;
            y = event.button.y;

            if (joinSelected) {
                game.stateID = 2; //changes the state to tell the Game.cpp loop to start the actual game
                play = false;
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
            if (x >= joinRect.x && x <= joinRect.x + joinRect.w &&
                    y >= joinRect.y && y <= joinRect.y + joinRect.h) {
                joinSelected = true;  //Activate the button
            } else {
                joinSelected = false; //deactivate the button
            }
            break;

        case SDL_WINDOWEVENT:
            switch (event.window.event) {
                case SDL_WINDOWEVENT_RESIZED:
                    //Re-render with the new size
                    //data1 --> new window width, | data2 --> new window height
                    screenRect = {0, 0, event.window.data1, event.window.data2};
                    break;
            }
            break;

        case SDL_QUIT:
            play = false;
            game.stateID = 0;
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
 */
void GameStateMenu::positionElements() {
    hostRect = {
        static_cast<int>(screenRect.w * 4 / 12), 
        static_cast<int>(screenRect.h * 1 / 2), 
        static_cast<int>(screenRect.w * 1 / 3), 
        static_cast<int>(screenRect.h * 1 / 14)};
    hostMagic.setRect(hostRect);
    userRect = {
        static_cast<int>(screenRect.w * 4 / 12), 
        static_cast<int>(screenRect.h * 9 / 14), 
        static_cast<int>(screenRect.w * 1 / 3), 
        static_cast<int>(screenRect.h * 1 / 14)};
    userMagic.setRect(userRect);
    joinRect = {
        static_cast<int>(screenRect.w * 5 / 12), 
        static_cast<int>(screenRect.h * 11 / 14), 
        static_cast<int>(screenRect.w * 1 / 6), 
        static_cast<int>(screenRect.h * 1 / 14)};
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
 */
void GameStateMenu::render() {
    //Only draw when not minimized
    if (game.window.isMinimized()) {
        return;
    }
    screenRect = {0, 0, game.window.getWidth(), game.window.getHeight()};

    //Position all screen elements in the window
    positionElements();

    //Clear screen
    SDL_RenderClear(Renderer::instance().getRenderer());

    //render the splash screen
    Renderer::instance().render(screenRect, TEXTURES::MAIN);

    //Join and Options text
    //Change the color of the text when active
    if (joinSelected) {
        joinMagic.setColor(RED);
    } else {
        joinMagic.setColor(BLACK);
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
