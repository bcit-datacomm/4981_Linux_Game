#ifndef CAMERA_H
#define CAMERA_H
#include <SDL2/SDL.h>

class Camera {
public:
    
    void setViewSize(float w, float h); // Set camera width and height
    void move(float x, float  y); // moves the camera based on player
    
    Camera(float w, float h);
    ~Camera();
    
    float getX();
    float getY();
    
private:
    
    SDL_Rect viewport; // Camera area of view
    
};

#endif
