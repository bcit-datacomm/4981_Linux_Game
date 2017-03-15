#ifndef WINDOW_H
#define WINDOW_H
#include <SDL2/SDL.h>
#include "../collision/HitBox.h"

//Screen dimension constants
const int SCREEN_WIDTH = 1280;
const int MIN_SCREEN_WIDTH = 960;
const int SCREEN_HEIGHT = 960;
const int MIN_SCREEN_HEIGHT = 720;
const int SCREEN_FPS = 60;
const int SCREEN_TICK_PER_FRAME = 1000 / SCREEN_FPS;

class Window {
public:
    //Intializes internals
    Window();

    //Creates window
    bool init();

    //Creates renderer from internal window
    SDL_Renderer* createRenderer();
    
    // Gets the scrren surface of the window
    SDL_Surface* getScreenSurface();

    //Handles window events
    void handleEvent( SDL_Event& e );

    //Deallocates internals
    void free();

    //Window dimensions
    int getWidth();
    int getHeight();

    //Window focii
    bool hasMouseFocus();
    bool hasKeyboardFocus();
    bool isMinimized();

    //gets mWindow
     SDL_Window* getWindow();

private:
    //Window data
    SDL_Window* mWindow;

    //Window dimensions
    int mWidth;
    int mHeight;

    //Window focus
    bool mMouseFocus;
    bool mKeyboardFocus;
    bool mFullScreen;
    bool mMinimized;
    
};


#endif
