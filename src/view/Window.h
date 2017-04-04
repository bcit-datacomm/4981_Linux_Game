#ifndef WINDOW_H
#define WINDOW_H

#include <SDL2/SDL.h>

#include "../collision/HitBox.h"


//Screen dimension constants
constexpr int SCREEN_WIDTH = 1280;
constexpr int MIN_SCREEN_WIDTH = 960;
constexpr int SCREEN_HEIGHT = 960;
constexpr int MIN_SCREEN_HEIGHT = 720;
constexpr int SCREEN_FPS = 60;
constexpr float SCREEN_TICK_PER_FRAME = 1000.0 / SCREEN_FPS;

class Window {
public:
    //Intializes internals
    Window();
    ~Window();

    //Creates window
    bool init();
    // Gets the scrren surface of the window
    SDL_Surface *getScreenSurface();

    //Handles window events
    void handleEvent(SDL_Event& e);

    int getWidth() const {return width;}
    int getHeight() const {return height;}
    bool hasMouseFocus() const {return mouseFocus;}
    bool hasKeyboardFocus() const {return keyboardFocus;}
    bool isMinimized() const {return minimized;}
    auto getWindow() const {return window;};

private:
    //Window data
    SDL_Window *window;

    //Window dimensions
    int width;
    int height;

    //Window focus
    bool mouseFocus;
    bool keyboardFocus;
    bool fullScreen;
    bool minimized;
};


#endif
