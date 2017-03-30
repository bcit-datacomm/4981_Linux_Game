#ifndef TEXTOMAGIC_H
#define TEXTOMAGIC_H

#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

/**
 * Designer: Isaac Morneau
 * Programmer: Isaac Morneau
 * Date: March 25, 2017
 * Notes:
 *  This class wraps the requirements for recalulating text for display so that the font size is
 *  not stretched. It was designed primariy for use with menus and user input as it handles
 *  changing text length and will internally recaluate all of the needed rectangles.
 *
 *  Overall it works as a wrapper to abstract the issues that arise when rendering fonts that use
 *  kerning instead of monospace leading to warped text.
 */
class Textomagic {
    public:
        Textomagic(const SDL_Rect& draw, TTF_Font* font, const int scaleLen = 0, 
            const std::string& initText = "", const SDL_Color& color = {255,255,255,255});
        ~Textomagic();

        void setRect(const SDL_Rect& draw);
        void setFont(TTF_Font* font);
        void setScale(const int scaleLen);
        void setText(const std::string& text);
        void setColor(const SDL_Color& color);
        void render();

    private:
        SDL_Rect draw;
        TTF_Font* font;
        int maxLen;
        float wScale, hScale;
        std::string text;
        SDL_Color color;

        //recalculate render destination
        void updateBox(const int w);
        //generate the texture
        void makeTex();
        //recalculate the kerning scale
        void updateScale();

        SDL_Texture *textTex;
        SDL_Rect textBox;

        //pointer pulled from Renderer.cpp
        SDL_Renderer *renderer;
};


#endif
