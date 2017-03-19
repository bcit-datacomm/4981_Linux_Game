#ifndef CAMERA_H
#define CAMERA_H
#include <SDL2/SDL.h>

class Camera {
public:

    void setViewSize(float w, float h); // Set camera width and height
    void move(float x, float  y); // moves the camera based on player

    Camera(float w, float h);
    ~Camera();

    float getX(){return viewport.x;};
    float getY(){return viewport.y;};
    float getW(){return viewport.w;};
    float getH(){return viewport.h;};
    SDL_Rect getViewport() {return viewport;};

private:

    SDL_Rect viewport; // Camera area of view

};

#endif
