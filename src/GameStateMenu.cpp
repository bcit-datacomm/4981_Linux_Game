#include <stdio.h>
#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "GameStateMenu.h"
#include "LTimer.h"
#include "LTexture.h"
#include "Window.h"
#include <unistd.h>

bool GameStateMenu::load() {

	bool success = true;

	this->menuFont = TTF_OpenFont( "assets/fonts/Overdrive Sunset.otf", 110);
	if ( this->menuFont == NULL ) {
		printf( "Failed to load font - Overdrive Sunset.otf! SDL_ttf Error: %s\n", TTF_GetError() );
		success = false;
	}

	this->headingFont = TTF_OpenFont( "assets/fonts/SEGUISB.ttf", 30);
	if ( this->headingFont == NULL ) {
		printf( "Failed to load font - SEGUISB.TTF! SDL_ttf Error: %s\n", TTF_GetError() );
		success = false;
	}

	this->textboxFont = TTF_OpenFont( "assets/fonts/SEGOEUISL.ttf", 30);
	if ( this->textboxFont == NULL ) {
		printf( "Failed to load font - SEGOEUISL.ttf! SDL_ttf Error: %s\n", TTF_GetError() );
		success = false;
	}

	this->level = new Level();
	if (!this->level->levelTexture.loadFromFile("assets/TitleScreen_Marz.png", this->game->renderer)) {
		printf("Failed to load the level texture!\n");
		success = false;
	} else {
		this->level->levelTexture.setDimensions(this->game->window->getWidth(), this->game->window->getHeight());
	}

	this->camera = new Camera(this->game->window->getWidth(), this->game->window->getHeight());


	return success;
}

void GameStateMenu::loop() {

	// State Loop
	while (this->play) {
		this->handle();	// Handle user input
		this->render();	// Render game state to window
	}
}

void GameStateMenu::sync() {

}

/**
* Function: handle
*
* Date:
* JF: February 8, 2017: added handler for window resizing event
*
* Designer:
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
	SDL_WaitEvent( &this->event );
	this->game->window->handleEvent(this->event);
		switch ( this->event.type ) {
      	case SDL_KEYDOWN:
            keyCode = this->event.key.keysym.sym;
            if (keyCode == SDLK_ESCAPE) {
                play = false;
                break;
            }

            for (int i = 0; i < NUM_TEXT_FIELDS; i++) {
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
            for (int i = 0; i < NUM_TEXT_FIELDS; i++) {
                if (activeTextbox[i]) {
                    if (!((this->event.text.text[ 0 ] == 'c' || this->event.text.text[ 0 ] == 'C') &&
                          (this->event.text.text[ 0 ] == 'v' || this->event.text.text[ 0 ] == 'V') &&
                           SDL_GetModState() & KMOD_CTRL)) {
                        if (textInput[i].length() < maxLength) {
                            textInput[i] += this->event.text.text;
                        }
                    }
                }
            }
            break;
      	case SDL_KEYUP:
       		switch ( this->event.key.keysym.sym ) {
				default:
	               	break;
			}
        	break;
        case SDL_MOUSEMOTION:
        	x = this->event.motion.x;
        	y = this->event.motion.y;
        	for (int i = 0; i < NUM_MENU_ITEMS; i++) {
        		if (x >= pos[i].x && x <= pos[i].x + this->menuTextTextures[i].getWidth() &&
        			y >= pos[i].y && y <= pos[i].y + this->menuTextTextures[i].getHeight()) {
                    selected[i] = true;
        		} else {
                    selected[i] = false;
        		}
        	}
        	break;
        case SDL_MOUSEBUTTONDOWN:
        	x = this->event.button.x;
        	y = this->event.button.y;
        	for (int i = 0; i < NUM_MENU_ITEMS; i++) {
                if (selected[i]) {
                    this->update(i);
        			play = false;
                }
        	}
        	for (int i = 0; i < NUM_TEXT_FIELDS; i++) {
        		if (x >= textboxPos[i].x && x <= textboxPos[i].x + textboxPos[i].w &&
        			y >= textboxPos[i].y && y <= textboxPos[i].y + textboxPos[i].h) {
                    activeTextbox[i] = true;
        		} else {
                    activeTextbox[i] = false;
        		}
        	}

        	break;
        case SDL_WINDOWEVENT:
        	switch (this->event.window.event) {
	    		case SDL_WINDOWEVENT_RESIZED:
	    			this->level->levelTexture.setDimensions(this->game->window->getWidth(),
	    													this->game->window->getHeight());
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

void GameStateMenu::update(const float& delta) {

	// TEMP: need to handle changing states better (use correct numbering in Game.cpp)
	//Add a switch statement instead
	if(delta == 0) {
		this->game->stateID = 2;
	}
}

void GameStateMenu::positionElements() {

    int         windowWidth     = this->game->window->getWidth();
    int         windowHeight    = this->game->window->getHeight();
    int         maxTextWidth    = 0;
    int         maxTextHeight   = 0;
    int         vertPadding     = 50;
    int         horzPadding     = 10;
    std::string longestString   = "";
    char        largestChar     = 'W';


    if(!TTF_WasInit() && TTF_Init()==-1) {
        printf("TTF_Init: %s\n", TTF_GetError());
        exit(1);
    }
    //Calculate the pixel length and height of the largest possible string
    longestString.resize(maxLength+1, largestChar);
	TTF_SizeText(textboxFont, longestString.c_str(), &maxTextWidth, &maxTextHeight);

    //Position the menu text
    pos[JOIN].x     = windowWidth/2  - this->menuTextTextures[JOIN].getWidth()/2;
    pos[JOIN].y     = windowHeight/2 + this->menuTextTextures[JOIN].getHeight();
    pos[OPTIONS].x  = windowWidth/2  - this->menuTextTextures[OPTIONS].getWidth()/2;
    pos[OPTIONS].y  = windowHeight/2 + this->menuTextTextures[OPTIONS].getHeight()*2.2;

    //Create a textbox for the server IP
    textboxPos[IP].w = maxTextWidth;
    textboxPos[IP].h = maxTextHeight;
    textboxPos[IP].x = windowWidth/2  - textboxPos[IP].w/2;
    textboxPos[IP].y = pos[JOIN].y - vertPadding;

    //Create a textbox for the Username
    textboxPos[USERNAME].w = maxTextWidth;
    textboxPos[USERNAME].h = maxTextHeight;
    textboxPos[USERNAME].x = windowWidth/2  - textboxPos[USERNAME].w/2;
    textboxPos[USERNAME].y = textboxPos[IP].y - vertPadding;

    //Position the Textbox titles to the left of their respective text box
    textboxTitlePos[IP].x = textboxPos[IP].x - this->textboxTitleTextures[IP].getWidth() - horzPadding;
    textboxTitlePos[IP].y = textboxPos[IP].y;
    textboxTitlePos[USERNAME].x = textboxPos[USERNAME].x - this->textboxTitleTextures[USERNAME].getWidth() - horzPadding;
    textboxTitlePos[USERNAME].y = textboxPos[USERNAME].y;

//    std::cout << "before textboxTitlePos[" << "IP" << "]X:" << textboxTitlePos[IP].x << "\n";
//    std::cout << "before textboxTitlePos[" << "IP" << "]Y:" << textboxTitlePos[IP].y << "\n";
}

void GameStateMenu::render() {
	//Only draw when not minimized
	if ( !this->game->window->isMinimized() ) {

		//Clear screen
		SDL_RenderClear( this->game->renderer );

 		//Render textures
		this->level->levelTexture.render(this->game->renderer, 0-this->camera->getX(), 0-this->camera->getY());

        //Position all screen elements in the window
		positionElements();
        //sleep(1);

		//Render Menu text
		for (int i = 0; i < NUM_MENU_ITEMS; i++) {
			if(selected[i]) {
//                renderText(this->menuTextTextures[i], this->menuItems[i], fontColors[GREEN], this->menuFont, pos[i]);

				if ( !this->menuTextTextures[i].loadFromRenderedText( this->menuItems[i],
											  fontColors[GREEN], this->game->renderer, this->menuFont ) ) {
					printf( "Unable to render menu text texture!\n" );
				}
			} else {
				if ( !this->menuTextTextures[i].loadFromRenderedText( this->menuItems[i],
											  fontColors[WHITE], this->game->renderer, this->menuFont ) ) {
					printf( "Unable to render menu text texture!\n" );
				}
//                renderText(this->menuTextTextures[i], this->menuItems[i], fontColors[WHITE], this->menuFont, pos[i]);
			}
			this->menuTextTextures[i].render(this->game->renderer, pos[i].x, pos[i].y);
		}

        //Change the color of the textbox when active
        //Used so User knows when textbox is can accept input
		for (int i = 0; i < NUM_TEXT_FIELDS; i++) {
            if (activeTextbox[i]) {
                SDL_SetRenderDrawColor(this->game->renderer, YELLOW_RGB[0], YELLOW_RGB[1], YELLOW_RGB[2], OPAQUE);
                SDL_RenderFillRect(this->game->renderer, &textboxPos[i]);
            } else {
                SDL_SetRenderDrawColor(this->game->renderer, WHITE_RGB[0], WHITE_RGB[1], WHITE_RGB[2], OPAQUE);
                SDL_RenderFillRect(this->game->renderer, &textboxPos[i]);
            }
		}

		for (int i = 0; i < NUM_TEXT_FIELDS; i++) {
            if (!textInput[i].empty()) {
//                renderText(this->textboxTextures[i], this->textInput[i].c_str(), fontColors[BLACK], this->textboxFont, textboxPos[i].x + 5, textboxPos[i].y - 7);

                if ( !this->textboxTextures[i].loadFromRenderedText( this->textInput[i],
                                              fontColors[BLACK], this->game->renderer, this->textboxFont ) ) {
                    printf( "Unable to render textbox text texture!\n" );
                }
                this->textboxTextures[i].render(this->game->renderer, textboxPos[i].x + 5, textboxPos[i].y - 7);
			}
		}

		//Render textbox titles
//		for (int i = 0; i < NUM_TEXT_FIELDS; i++) {
////            renderText(this->textboxTitleTextures[i], this->textboxTitles[i], fontColors[WHITE], this->headingFont, textboxTitlePos[i].x, textboxTitlePos[i].y);
//            if ( !this->textboxTitleTextures[i].loadFromRenderedText( this->textboxTitles[i],
//                                        fontColors[WHITE], this->game->renderer, this->headingFont ) ) {
//                printf( "Unable to render textbox texture!\n" );
//            }
//
//			this->textboxTitleTextures[i].render(this->game->renderer, textboxTitlePos[i].x, textboxTitlePos[i].y);
//		}

		//Update screen
		SDL_RenderPresent( this->game->renderer );
	}
}

void GameStateMenu::renderText(LTexture fontTexture, const char* text, SDL_Color color, TTF_Font* font, int x, int y) {
    if ( !fontTexture.loadFromRenderedText( text, color, this->game->renderer, font ) ) {
                printf( "Unable to render text texture!\n" );
    }
    fontTexture.render(this->game->renderer, x, y);
}

GameStateMenu::~GameStateMenu() {

	// Free texture and font
	this->menuTextTextures[1].free();
	this->menuTextTextures[0].free();
	TTF_CloseFont(this->textboxFont);
	TTF_CloseFont(this->headingFont);
	TTF_CloseFont(this->menuFont);
	this->menuFont = NULL;
	this->textboxFont = NULL;
	this->headingFont = NULL;

}
