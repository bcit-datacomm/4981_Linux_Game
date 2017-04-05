#ifndef CAMERA_H
#define CAMERA_H
#include <SDL2/SDL.h>

class Camera {
public:

    void setViewSize(const int w, const int h); // Set camera width and height
    void move(const int x, const int y); // moves the camera based on player

    Camera(const int w, const int h);
    ~Camera() = default;

    int getX() const {return viewport.x;}
    int getY() const {return viewport.y;}
    int getW() const {return viewport.w;}
    int getH() const {return viewport.h;}
    SDL_Rect& getViewport() {return viewport;}

private:
    SDL_Rect viewport; // Camera area of view

};

#endif
