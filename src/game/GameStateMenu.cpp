#include <stdio.h>
#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "GameStateMenu.h"
#include "../basic/LTimer.h"
#include "../sprites/LTexture.h"
#include "../view/Window.h"
#include <unistd.h>


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
* Interface: GameStateMenu(Game& g, int gameWidth, int gameHeight)
*                           Game& g: The instance of the game which the display window is tied to
*                           int gameWidth: Width of the game window
*                           int gameHeight: Height of the game window
*
* Notes:
* GameStateMenu ctor which initializes all member variables for use throughout class
*/
GameStateMenu::GameStateMenu(Game& g, int gameWidth, int gameHeight):
                                GameState(g),
                                headingFont(nullptr), textboxFont(nullptr), menuFont(nullptr),
                                level(),
                                camera(gameWidth,gameHeight),
                                menuItems({"join", "options"}),
                                selected{false,false},
                                activeTextbox{false,false},
                                defaultText({"IP Address", "Username"}),
                                textInput({defaultText[IP], defaultText[USERNAME]}),
                                fontColors({SDL_WHITE_RGB, SDL_GREEN_RGB, SDL_BLACK_RGB, SDL_RED_RGB}){
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
* Interface: load()
*
* Returns: true if the level, camera, and all font textures were loaded without error
*
* Notes:
* Function loads the initial state of the window
* Opens all fonts to be used, the title screen background, and the camera for viewing the window
*/
bool GameStateMenu::load() {
    bool success = true;

    menuFont = TTF_OpenFont( "assets/fonts/Overdrive Sunset.otf", 110);
    if ( menuFont == nullptr ) {
        printf( "Failed to load font - Overdrive Sunset.otf! SDL_ttf Error: %s\n", TTF_GetError() );
        success = false;
    }

    headingFont = TTF_OpenFont( "assets/fonts/SEGUISB.ttf", 30);
    if ( headingFont == nullptr ) {
        printf( "Failed to load font - SEGUISB.TTF! SDL_ttf Error: %s\n", TTF_GetError() );
        success = false;
    }

    textboxFont = TTF_OpenFont( "assets/fonts/SEGOEUISL.ttf", 30);
    if ( textboxFont == nullptr ) {
        printf( "Failed to load font - SEGOEUISL.ttf! SDL_ttf Error: %s\n", TTF_GetError() );
        success = false;
    }

    if (!level.levelTexture.loadFromFile("assets/TitleScreen_Marz.png", game.renderer)) {
        printf("Failed to load the level texture!\n");
        success = false;
    } else {
        level.levelTexture.setDimensions(game.window.getWidth(), game.window.getHeight());
    }
    return success;
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
* JF: February 8, 2017: added handler for window resizing event
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
 int x,y;
 SDL_Keycode keyCode;

 //Handle events on queue
 SDL_WaitEvent( &event );
 game.window.handleEvent(event);
    switch ( event.type ) {
        case SDL_KEYDOWN:
            keyCode = event.key.keysym.sym;
            if (keyCode == SDLK_ESCAPE) {
                play = false;
                break;
            }

            for (size_t i = 0; i < NUM_TEXT_FIELDS; i++) {
                if (activeTextbox[i]) {
                    //Handling backspace
                    if (keyCode == SDLK_BACKSPACE && textInput[i].length() > 0) {
                        textInput[i].pop_back();
                    }
                    //Handle copy
                    else if (keyCode == SDLK_c && SDL_GetModState() & KMOD_CTRL) {
                        SDL_SetClipboardText( textInput[i].c_str() );
                    }
                    //Handle paste
                    else if (keyCode == SDLK_v && SDL_GetModState() & KMOD_CTRL) {
                        textInput[i] = SDL_GetClipboardText();
                    }
                }
            }
        break;
        case SDL_TEXTINPUT:
            for (size_t i = 0; i < NUM_TEXT_FIELDS; i++) {
                if (activeTextbox[i]) {
                    //Ensures that a copy or paste is not occurring
                    if (!((event.text.text[ 0 ] == 'c' || event.text.text[ 0 ] == 'C') &&
                          (event.text.text[ 0 ] == 'v' || event.text.text[ 0 ] == 'V') &&
                           SDL_GetModState() & KMOD_CTRL)) {
                        //Update the string int displayed int he textbox
                        if (textInput[i].length() < maxLength) {
                            textInput[i] += event.text.text;
                        }
                    }
                }
            }
            break;
        case SDL_KEYUP: //Do nothing on key release
            break;
        case SDL_MOUSEMOTION:
            x = event.motion.x;
            y = event.motion.y;
            for (size_t i = 0; i < NUM_MENU_ITEMS; i++) {
                //Check if mouse is above one of the menu uptions
                if (x >= menuItemPos[i].x && x <= menuItemPos[i].x + menuTextTextures[i].getWidth() &&
                    y >= menuItemPos[i].y && y <= menuItemPos[i].y + menuTextTextures[i].getHeight()) {
                    selected[i] = true;  //Activate the button
                } else {
                    selected[i] = false; //deactivate the button
                }
            }
            break;
        case SDL_MOUSEBUTTONDOWN:
            x = event.button.x;
            y = event.button.y;
            for (size_t i = 0; i < NUM_MENU_ITEMS; i++) {
                if (selected[i]) {
                    update(i);
                    play = false;
                }
            }
            for (size_t i = 0; i < NUM_TEXT_FIELDS; i++) {
                //Check if the mouse was clicked inside the textbox area
                if (x >= textboxPos[i].x && x <= textboxPos[i].x + textboxPos[i].w &&
                    y >= textboxPos[i].y && y <= textboxPos[i].y + textboxPos[i].h) {
                    activeTextbox[i] = true; //Activate the textbox to enable typing
                    //If default string is detected, delete it
                    if (textInput[i].compare(defaultText[i]) == 0) {
                        textInput[i] = "";
                    }
                } else {
                    activeTextbox[i] = false; //deactivate the textbox
                    //Reset textbox text to default text, if textbox is empty
                    if (textInput[i].empty()) {
                        textInput[i] = defaultText[i];
                    }
                }
            }
            break;
        case SDL_WINDOWEVENT:
            switch (event.window.event) {
                case SDL_WINDOWEVENT_RESIZED:
                    //Adjust the dimensions of the window if resized
                    level.levelTexture.setDimensions(game.window.getWidth(), game.window.getHeight());
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

    if(delta == JOIN) {
        game.stateID = 2;
    } else if (delta == OPTIONS) {
        game.stateID = 2; //TEMPORARY: change to correct state ID once implemented
    } else {
        game.stateID = 0;
    }
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
* Interface: positionElements()
*
* Returns: void
*
* Notes:
* Function positions all screen elements in the window
*/
void GameStateMenu::positionElements() {

    int windowWidth = game.window.getWidth();
    int windowHeight = game.window.getHeight();
    int maxTextWidth = 0;
    int maxTextHeight = 0;
    int vertPadding = 50;
    int horzPadding = 10;
    std::string longestString = "";
    char largestChar = 'W';


    //Check if TTF was initialized correctly
    if(!TTF_WasInit() && TTF_Init()== -1) {
        printf("TTF_Init: %s\n", TTF_GetError());
        exit(1);
    }
    //Calculate the pixel length and height of the largest possible string
    longestString.resize(maxLength + 1, largestChar);
    TTF_SizeText(textboxFont, longestString.c_str(), &maxTextWidth, &maxTextHeight);

    //Position the menu text
    menuItemPos[JOIN].x = windowWidth / 2  - menuTextTextures[JOIN].getWidth() / 2;
    menuItemPos[JOIN].y = windowHeight / 2 + menuTextTextures[JOIN].getHeight();
    menuItemPos[OPTIONS].x = windowWidth / 2  - menuTextTextures[OPTIONS].getWidth() / 2;
    menuItemPos[OPTIONS].y = windowHeight / 2 + menuTextTextures[OPTIONS].getHeight() * 2.2;

    //Create a textbox for the server IP
    textboxPos[IP].w = maxTextWidth;
    textboxPos[IP].h = maxTextHeight;
    textboxPos[IP].x = windowWidth / 2  - textboxPos[IP].w / 2;
    textboxPos[IP].y = menuItemPos[JOIN].y - vertPadding;

    //Create a textbox for the Username
    textboxPos[USERNAME].w = maxTextWidth;
    textboxPos[USERNAME].h = maxTextHeight;
    textboxPos[USERNAME].x = windowWidth / 2  - textboxPos[USERNAME].w / 2;
    textboxPos[USERNAME].y = textboxPos[IP].y - vertPadding;

    //position the text for the IP Address textbox
    textboxTextPos[IP].x = textboxPos[IP].x + horzPadding;
    textboxTextPos[IP].y = textboxPos[IP].y - ((textboxPos[IP].h - maxTextHeight) / 2);

    //position the text for the Username textbox
    textboxTextPos[USERNAME].x = textboxPos[USERNAME].x + horzPadding;
    textboxTextPos[USERNAME].y = textboxPos[USERNAME].y - ((textboxPos[USERNAME].h - maxTextHeight) / 2);
}


/**
* Function: render
*
* Date:
* JF: February 10, 2017: created function
* JF: March 7-8, 2017: Moved functionality for rendering to wrapper function
*
* Designer:
* Jacob Frank
*
* Programmer:
* Jacob Frank
*
* Interface: render()
*
* Returns: void
*
* Notes:
* Function renders all assets to the screen
* Changes the color of any assets that are selected
* Calls helper function to position elements in the window
*/
void GameStateMenu::render() {
    //Only draw when not minimized
    if (!game.window.isMinimized()) {

        //Clear screen
        SDL_RenderClear( game.renderer );

        //Render textures
        level.levelTexture.render(game.renderer, 0 - camera.getX(), 0 - camera.getY());

        //Position all screen elements in the window
        positionElements();

        //Render Menu option text
        for (size_t i = 0; i < NUM_MENU_ITEMS; i++) {
            if(selected[i]) {
                renderText(&menuTextTextures[i], menuItems[i].c_str(), fontColors[GREEN], menuFont, menuItemPos[i]);
            } else {
                renderText(&menuTextTextures[i], menuItems[i].c_str(), fontColors[RED], menuFont, menuItemPos[i]);
            }
            menuTextTextures[i].render(game.renderer, menuItemPos[i].x, menuItemPos[i].y);
        }

        //Change the color of the textbox when active
        //Used so User knows when textbox is can accept input
        for (size_t i = 0; i < NUM_TEXT_FIELDS; i++) {
            if (activeTextbox[i]) {
                SDL_SetRenderDrawColor(game.renderer, LT_GREEN_RGB[0], LT_GREEN_RGB[1], LT_GREEN_RGB[2], OPAQUE);
                SDL_RenderFillRect(game.renderer, &textboxPos[i]);
            } else {
                SDL_SetRenderDrawColor(game.renderer, WHITE_RGB[0], WHITE_RGB[1], WHITE_RGB[2], OPAQUE);
                SDL_RenderFillRect(game.renderer, &textboxPos[i]);
            }
        }

        //Render the Textfield text to the screen
        for (size_t i = 0; i < NUM_TEXT_FIELDS; i++) {
            if (!textInput[i].empty()) {
                renderText(&textboxTextures[i], textInput[i].c_str(), fontColors[BLACK],
                textboxFont, textboxTextPos[i]);
            }
        }

        //Update screen
        SDL_RenderPresent(game.renderer);
    }
}

/**
* Function: renderText
*
* Date:
* JF: March 7, 2017: Created wrapper function for loadFromRenderText function
*
* Designer:
* Jacob Frank
*
* Programmer:
* Jacob Frank
*
* Interface: renderText(LTexture *fontTexture, const char* text, SDL_Color color, TTF_Font* font, SDL_Rect rect)
*                       LTexture *fontTexture: Wrapper for the font texture to be rendered to the screen window
*                       const char* text: Text to be rendered to the screen
*                       SDL_Color color: Color of the text to be rendered
*                       TTF_Font* font: The font to be used to write the text
*                       SDL_Rect rect: Structure that defines the bounding box of the rectangle for the text
*
* Returns: void
*
*
* Notes:
* Wrapper function for the loadFromRenderedText function.
* When called, it renders the passed in text to the screen
*/
void GameStateMenu::renderText(LTexture *fontTexture, const char* text,
                                SDL_Color color, TTF_Font* font, SDL_Rect rect) {
    if ( !fontTexture->loadFromRenderedText( text, color, game.renderer, font ) ) {
        printf( "Unable to render text texture!\n" );
    }
    fontTexture->render(game.renderer, rect.x, rect.y);
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
* Interface: ~GameStateMenu()
*
* Notes:
* Deconstructor, frees all allocated memory for textures and fonts
*/
GameStateMenu::~GameStateMenu() {
    // Free texture and font
    menuTextTextures[1].free();
    menuTextTextures[0].free();
    TTF_CloseFont(textboxFont);
    TTF_CloseFont(headingFont);
    TTF_CloseFont(menuFont);
    menuFont = nullptr;
    textboxFont = nullptr;
    headingFont = nullptr;
}
