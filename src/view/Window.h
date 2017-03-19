#ifndef WINDOW_H
#define WINDOW_H
#include <SDL2/SDL.h>
#include "../collision/HitBox.h"

//Screen dimension constexprants
constexpr int SCREEN_WIDTH = 1280;
constexpr int MIN_SCREEN_WIDTH = 960;
constexpr int SCREEN_HEIGHT = 960;
constexpr int MIN_SCREEN_HEIGHT = 720;
constexpr int SCREEN_FPS = 60;
constexpr int SCREEN_TICK_PER_FRAME = 1000 / SCREEN_FPS;

class Window {
public:
    //Intializes internals
    Window();

    //Creates window
    bool init();

    // Gets the scrren surface of the window
    SDL_Surface* getScreenSurface();

    //Handles window events
    void handleEvent(SDL_Event& e);

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
