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
*/
GameStateMenu::GameStateMenu(Game& g):GameState(g), headingFont(nullptr), textboxFont(nullptr),
    menuFont(nullptr), screenRect{ZERO, ZERO, game.window.getWidth(), game.window.getHeight()} {
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
*/

bool GameStateMenu::load() {
    logv("Loading Fonts...\n");

    if ((menuFont = Renderer::instance().loadFont("assets/fonts/Overdrive Sunset.otf", 
            110)) == nullptr) {
        return false;
    }

    Renderer::instance().createText(TEXTURES::JOIN_FONT, menuFont, "Join", 
        SDL_Color{MAX_RGB, MAX_RGB, MAX_RGB, MAX_RGB});
    Renderer::instance().createText(TEXTURES::OPTIONS_FONT, menuFont, "Options", 
        SDL_Color{MAX_RGB, MAX_RGB, MAX_RGB, MAX_RGB});

    if ((headingFont = Renderer::instance().loadFont("assets/fonts/SEGUISB.ttf", 
            FONT_SIZE)) == nullptr) {
        return false;
    }

    if ((textboxFont = Renderer::instance().loadFont("assets/fonts/SEGOEUISL.ttf", 
            FONT_SIZE)) == nullptr) {
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
    //int x, y; // <-- leaving these in for future use

    SDL_Keycode keyCode;

    //Handle events on queue
    SDL_WaitEvent(&event);
    game.window.handleEvent(event);

    switch (event.type) {

        case SDL_MOUSEBUTTONDOWN:
            // x = event.button.x;
            // y = event.button.y;

            //move to the game when a click occurs
            //changes the state to tell the Game.cpp loop to start the actual game
            game.stateID = 2;

            //breaks out of the menu loop and Game.cpp re-evaluates the state
            play = false;
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
        //intentionally left blank for now
        //only temporary, will be functional later
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
*/
void GameStateMenu::positionElements() {

    int maxTextWidth = ZERO;
    int maxTextHeight = ZERO;
    int vertPadding = 100;

    std::string longestString = "";
    char largestChar = 'W';

    //Check if TTF was initialized correctly
    if(!TTF_WasInit() && TTF_Init()== -1) {
        logv("TTF_Init: %s\n", TTF_GetError());
        exit(1);
    }

    //Calculate the pixel length and height of the largest possible string
    longestString.resize(maxLength + 1, largestChar);
    TTF_SizeText(textboxFont, longestString.c_str(), &maxTextWidth, &maxTextHeight);


    //Create a textbox for the server IP
    hostIPTextBox.x = (screenRect.w - hostIPTextBox.w) * 0.5;
    hostIPTextBox.y = screenRect.h * 0.5;
    hostIPTextBox.w = maxTextWidth;
    hostIPTextBox.h = maxTextHeight;

    //Create a textbox for the Username
    usernameTextBox.x = (screenRect.w - usernameTextBox.w) * 0.5;
    usernameTextBox.y = hostIPTextBox.y - vertPadding * 0.5;
    usernameTextBox.w = maxTextWidth;
    usernameTextBox.h = maxTextHeight;

    //Position the menu text
    joinRect.x = hostIPTextBox.x + hostIPTextBox.x * 0.25;
    joinRect.y = hostIPTextBox.y + vertPadding;
    joinRect.w = maxTextWidth * 0.5;
    joinRect.h = maxTextHeight * 2;

    optionsRect.x = hostIPTextBox.x;
    optionsRect.y = joinRect.y + vertPadding;
    optionsRect.w = maxTextWidth;
    optionsRect.h = maxTextHeight * 2;
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
        SDL_RenderClear(Renderer::instance().getRenderer());

        //render the splash screen
        Renderer::instance().render(screenRect, TEXTURES::MAIN);

        //Position all screen elements in the window
        positionElements();

        //textboxes
        Renderer::instance().render(usernameTextBox, TEXTURES::TEXTBOX);
        Renderer::instance().render(hostIPTextBox, TEXTURES::TEXTBOX);

        //Join and Options text
        Renderer::instance().render(joinRect, TEXTURES::JOIN_FONT);
        Renderer::instance().render(optionsRect, TEXTURES::OPTIONS_FONT);

        //Update screen
        SDL_RenderPresent(Renderer::instance().getRenderer());
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
