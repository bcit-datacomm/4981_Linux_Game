#ifndef LEVEL_H
#define LEVEL_H
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <stdio.h>
#include <string>
#include "../sprites/LTexture.h"

class Level {
public:
    std::string getTexturePath();
    LTexture levelTexture;
    
    Level();
    ~Level();
    
private:
    static int width;
    static int height;
    SDL_Surface* levelImg;

};

#endif
