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
* Re-added non-excessive initializations (Jacob Frank / March 26, 2017)
*/
GameStateMenu::GameStateMenu(Game& g):GameState(g), headingFont(nullptr), textboxFont(nullptr),
    menuFont(nullptr), screenRect{ZERO, ZERO, game.window.getWidth(), game.window.getHeight()}, joinSelected(false), optionsSelected(false), hostIPSelected(false), usernameSelected(false),
    hostIpDefaultText("IP Address"), usernameDefaultText("Username"),
    hostIpTextInput(hostIpDefaultText), usernameTextInput(usernameDefaultText){
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
* Changed the font texture and added Textures for active fonts (Jacob Frank / March 26, 2017)
*/

bool GameStateMenu::load() {
    logv("Loading Fonts...\n");

    if ((menuFont = Renderer::instance()->loadFont("assets/fonts/Overdrive Sunset.otf", 110)) == nullptr) {
        return false;
    }

    Renderer::instance()->createText(TEXTURES::JOIN_FONT, menuFont, "join", SDL_RED_RGB);
    Renderer::instance()->createText(TEXTURES::OPTIONS_FONT, menuFont, "options", SDL_RED_RGB);
    Renderer::instance()->createText(TEXTURES::JOIN_FONT_ACTIVE, menuFont, "join", SDL_GREEN_RGB);
    Renderer::instance()->createText(TEXTURES::OPTIONS_FONT_ACTIVE, menuFont, "options", SDL_GREEN_RGB);

    if ((headingFont = Renderer::instance()->loadFont("assets/fonts/SEGUISB.ttf", FONT_SIZE)) == nullptr) {
        return false;
    }

    if ((textboxFont = Renderer::instance()->loadFont("assets/fonts/SEGOEUISL.ttf", FONT_SIZE)) == nullptr) {
        return false;
    }

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
* Interface: handle()
*
* Returns: void
*
* Notes:
* Function handles events occuring within the window to control
* functionality of the main menu
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
            if (x >= hostIPTextBox.x && x <= hostIPTextBox.x + hostIPTextBox.w &&
                y >= hostIPTextBox.y && y <= hostIPTextBox.y + hostIPTextBox.h) {
                hostIPSelected = true;  //Activate the button

                //If default string is detected, delete it
                if (hostIpTextInput.compare(hostIpDefaultText) == 0) {
                    hostIpTextInput = "";
                }
            } else {
                hostIPSelected = false; //deactivate the button

                //Reset textbox text to default text, if textbox is empty
                if (hostIpTextInput.empty()) {
                    hostIpTextInput = hostIpDefaultText;
                }
            }

            //Checks the mouse position when the mouse click occurred and sets the username textbox
            //to it's acive state when the mouse button down event occurred within the textbox's bounds
            if (x >= usernameTextBox.x && x <= usernameTextBox.x + usernameTextBox.w &&
                y >= usernameTextBox.y && y <= usernameTextBox.y + usernameTextBox.h) {
                usernameSelected = true;  //Activate the button

                //If default string is detected, delete it
                if (usernameTextInput.compare(usernameDefaultText) == 0) {
                    usernameTextInput = "";
                }
            } else {
                usernameSelected = false; //deactivate the button

                //Reset textbox text to default text, if textbox is empty
                if (usernameTextInput.empty()) {
                    usernameTextInput = usernameDefaultText;
                }
            }
            break;

        case SDL_KEYDOWN:
            keyCode = event.key.keysym.sym;

            if (keyCode == SDLK_ESCAPE) {
                play = false;
            }
            break;

        case SDL_TEXTINPUT:
        //intentionally left blank for now
        //only temporary, will be functional later
            break;

        case SDL_KEYUP: //Do nothing on key release
        //intentionally left blank for now
        //only temporary, will be functional later
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

            //Checks the mouse position and sets the menu option Options
            //to it's acive state when the mouse hovers above the Options text
            if (x >= optionsRect.x && x <= optionsRect.x + optionsRect.w &&
                y >= optionsRect.y && y <= optionsRect.y + optionsRect.h) {
                optionsSelected = true;  //Activate the button
            } else {
                optionsSelected = false; //deactivate the button
            }
            break;

        case SDL_WINDOWEVENT:
            switch (event.window.event) {
                case SDL_WINDOWEVENT_RESIZED:
                    //Re-render with the new size
                    //data1 --> new window width, | data2 --> new window height
                    screenRect = {ZERO, ZERO, event.window.data1, event.window.data2};
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
* Interface: update(const float& delta)
*
* Returns: void
*
* Notes:
* Function positions all screen elements in the window
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
* Fixed positional data from restructuring not setting assets in correct position (Jacob Frank / March 26, 2017)
*/
void GameStateMenu::positionElements() {

    int maxTextWidth = ZERO;
    int maxTextHeight = ZERO;
    int vertPadding = 50;

    std::string longestString = "";
    char largestChar = 'W';

    std::string join = "join";
    int joinWidth = ZERO;
    int joinHeight = ZERO;

    std::string options = "options";
    int optionsWidth = ZERO;
    int optionsHeight = ZERO;

    //Check if TTF was initialized correctly
    if(!TTF_WasInit() && TTF_Init()== -1) {
        logv("TTF_Init: %s\n", TTF_GetError());
        exit(1);
    }

    //Calculate the pixel length and height of the largest possible string
    longestString.resize(maxLength + 1, largestChar);
    TTF_SizeText(textboxFont, longestString.c_str(), &maxTextWidth, &maxTextHeight);

    //Calculate the pixel size the menu options
    TTF_SizeText(menuFont, join.c_str(), &joinWidth, &joinHeight);
    TTF_SizeText(menuFont, options.c_str(), &optionsWidth, &optionsHeight);

    //Position the Join text
    joinRect.w = joinWidth;
    joinRect.h = joinHeight;
    joinRect.x = screenRect.w / 2 - joinRect.w / 2;
    joinRect.y = screenRect.h / 2 + joinRect.h;

    //Position the Options text
    optionsRect.w = optionsWidth;
    optionsRect.h = optionsHeight;
    optionsRect.x = screenRect.w / 2 - optionsRect.w / 2;
    optionsRect.y = screenRect.h / 2 + optionsRect.h * 2.2;

    //Create a textbox for the server IP
    hostIPTextBox.w = maxTextWidth;
    hostIPTextBox.h = maxTextHeight;
    hostIPTextBox.x = screenRect.w / 2 - hostIPTextBox.w / 2;
    hostIPTextBox.y = joinRect.y - vertPadding;

    //Create a textbox for the Username
    usernameTextBox.w = maxTextWidth;
    usernameTextBox.h = maxTextHeight;
    usernameTextBox.x = screenRect.w / 2 - usernameTextBox.w / 2;
    usernameTextBox.y = hostIPTextBox.y - vertPadding;
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
*/
void GameStateMenu::render() {
    //Only draw when not minimized
    if (!game.window.isMinimized()) {

        screenRect = {ZERO, ZERO, game.window.getWidth(), game.window.getHeight()};

        //Clear screen
        SDL_RenderClear(Renderer::instance()->getRenderer());

        //render the splash screen
        Renderer::instance()->render(screenRect, TEXTURES::MAIN);

        //Position all screen elements in the window
        positionElements();

        //textboxes
//        Renderer::instance()->render(usernameTextBox, TEXTURES::TEXTBOX);
//        Renderer::instance()->render(hostIPTextBox, TEXTURES::TEXTBOX);

        //Join and Options text
        //Change the color of the text when active
        if (joinSelected) {
            Renderer::instance()->render(joinRect, TEXTURES::JOIN_FONT_ACTIVE);
        } else {
            Renderer::instance()->render(joinRect, TEXTURES::JOIN_FONT);
        }

        if (optionsSelected) {
            Renderer::instance()->render(optionsRect, TEXTURES::OPTIONS_FONT_ACTIVE);
        } else {
            Renderer::instance()->render(optionsRect, TEXTURES::OPTIONS_FONT);
        }

        //Host IP and Username textboxes
        //Change the color of the textbox when active
        //Used so User knows when textbox is can accept input
        if (hostIPSelected) {
            Renderer::instance()->render(hostIPTextBox, TEXTURES::TEXTBOX_ACTIVE);
        } else {
            Renderer::instance()->render(hostIPTextBox, TEXTURES::TEXTBOX);
        }

        if (usernameSelected) {
            Renderer::instance()->render(usernameTextBox, TEXTURES::TEXTBOX_ACTIVE);
        } else {
            Renderer::instance()->render(usernameTextBox, TEXTURES::TEXTBOX);
        }

        if (!hostIpTextInput.empty()) {
            Renderer::instance()->createTempText(textboxFont, hostIpTextInput, SDL_BLACK_RGB);
        }

        if (!usernameTextInput.empty()) {
            Renderer::instance()->createTempText(textboxFont, usernameTextInput, SDL_BLACK_RGB);
        }

        //Update screen
        SDL_RenderPresent(Renderer::instance()->getRenderer());
    }
}

/**
* Function: ~GameStateMenu dtor
*
* Date:
* JF: February 8, 2017: Created
*
* Designer:
* Jacob Frank
*
* Programmer:
* Jacob Frank
*
* Modified by:
* Michael Goll (March 16, 2017)
*
* Interface: ~GameStateMenu()
*
* Notes:
* Deconstructor, frees all allocated memory for textures and fonts
*
* Revisions:
* Removed unnecessary free calls (Michael Goll / March 16, 2017)
*/
GameStateMenu::~GameStateMenu() {
    TTF_CloseFont(textboxFont);
    TTF_CloseFont(headingFont);
    TTF_CloseFont(menuFont);
}
