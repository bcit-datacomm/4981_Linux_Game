#ifndef TEXTOMAGIC_H
#define TEXTOMAGIC_H

#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

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
        int scale;
        std::string text;
        SDL_Color color;

        void updateBox();
        void makeTex();
        SDL_Texture *textTex;
        SDL_Rect textBox;

        //pointer pulled from Renderer.cpp
        SDL_Renderer *renderer;
};


#endif
