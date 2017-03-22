#ifndef CAMERA_H
#define CAMERA_H
#include <SDL2/SDL.h>

class Camera {
public:

    void setViewSize(int w, int h); // Set camera width and height
    void move(int x, int  y); // moves the camera based on player

    Camera(int w, int h);
    ~Camera();

    int getX(){return viewport.x;};
    int getY(){return viewport.y;};
    int getW(){return viewport.w;};
    int getH(){return viewport.h;};
    SDL_Rect getViewport() {return viewport;};

private:
    SDL_Rect viewport; // Camera area of view

};

#endif
