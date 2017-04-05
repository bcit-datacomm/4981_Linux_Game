#include <stdio.h>
#include <string>
#include <sstream>

#include "Window.h"

/**
* Date: Jan. 28, 2017
* Author: Jacob McPhail
* Modified: ---
* Function Interface: Window()
* Description: 
*   ctor for the window.
*/
Window::Window() : window(nullptr), width(0), height(0), mouseFocus(false), keyboardFocus(false),
        fullScreen(false), minimized(false) {

}

/**
* Date: Jan. 28, 2017
* Author: Jacob McPhail
* Modified: ---
* Function Interface: ~Window() 
* Description: 
*   dctor for the window, closes program window.
*/
Window::~Window() {
    if (window) {
        SDL_DestroyWindow(window);
    }

    mouseFocus = false;
    keyboardFocus = false;
    width = 0;
    height = 0;
}

/**
* Date: Jan. 28, 2017
* Author: Jacob McPhail
* Modified: ---
* Function Interface: init()
* Description: 
*   Spawns program window.
*/
bool Window::init() {
    //Create window
    window = SDL_CreateWindow( "4981 Linux Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE );
    if (window != nullptr) {
        mouseFocus = true;
        keyboardFocus = true;
        width = SCREEN_WIDTH;
        height = SCREEN_HEIGHT;
        return true;
    }
    return false;
}

/**
* Date: Jan. 28, 2017
* Author: Jacob McPhail
* Modified: ---
* Function Interface: getScreenSurface()
* Description: 
*   Getter for window surface.
*/
SDL_Surface* Window::getScreenSurface() {
    return SDL_GetWindowSurface(window);
}

/**
* Date: Jan. 28, 2017
* Author: Jacob McPhail
* Modified: ---
* Function Interface: handleEvent(SDL_Event& e)
*       e : Event from user input
*
* Description: 
*       Handles user input window events. 
*/
void Window::handleEvent(SDL_Event& e) {
    //Window event occured
    if (e.type == SDL_WINDOWEVENT) {
        switch(e.window.event) {
            //Get new dimensions
            case SDL_WINDOWEVENT_SIZE_CHANGED:
                if (e.window.data1 < MIN_SCREEN_WIDTH) {
                    SDL_SetWindowSize(window, MIN_SCREEN_WIDTH, e.window.data2);
                    width = MIN_SCREEN_WIDTH;
                } else {
                    width = e.window.data1;
                }
                if (e.window.data2 < MIN_SCREEN_HEIGHT) {
                    SDL_SetWindowSize(window, e.window.data1, MIN_SCREEN_HEIGHT);
                    height = MIN_SCREEN_HEIGHT;
                } else {
                    height = e.window.data2;
                }
                break;
                //Repaint on exposure
            case SDL_WINDOWEVENT_EXPOSED:
                break;
                //Mouse entered window
            case SDL_WINDOWEVENT_ENTER:
                mouseFocus = true;
                break;
                //Mouse left window
            case SDL_WINDOWEVENT_LEAVE:
                mouseFocus = false;
                break;
                //Window has keyboard focus
            case SDL_WINDOWEVENT_FOCUS_GAINED:
                keyboardFocus = true;
                break;
                //Window lost keyboard focus
            case SDL_WINDOWEVENT_FOCUS_LOST:
                keyboardFocus = false;
                break;
                //Window minimized
            case SDL_WINDOWEVENT_MINIMIZED:
                minimized = true;
                break;
                //Window maxized
            case SDL_WINDOWEVENT_MAXIMIZED:
                minimized = false;
                break;
                //Window restored
            case SDL_WINDOWEVENT_RESTORED:
                minimized = false;
                break;
        }
    } else if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_TAB) {
        //Enter exit full screen on return key
        if (fullScreen) {
            SDL_SetWindowFullscreen(window, SDL_FALSE);
            fullScreen = false;
        } else {
            SDL_DisplayMode DM;
            SDL_GetDesktopDisplayMode(0, &DM);
            SDL_SetWindowSize(window, DM.w, DM.h);
            SDL_SetWindowFullscreen(window, SDL_TRUE);
            fullScreen = true;
            minimized = false;
        }
    }
}
