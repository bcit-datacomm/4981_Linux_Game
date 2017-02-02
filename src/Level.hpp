#ifndef LEVEL_HPP
#define LEVEL_HPP

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <stdio.h>
#include <string>
#include "LTexture.hpp"

class Level
{
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
