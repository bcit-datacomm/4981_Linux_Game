#include "Textomagic.h"
#include "Renderer.h"

Textomagic::Textomagic(const SDL_Rect& draw, TTF_Font* font, const int scaleLen,
        const std::string& initText, const SDL_Color& color)
        :draw(draw), font(font), scale(scaleLen), text(initText), color(color), textTex(nullptr),
        textBox(draw), renderer(Renderer::instance().getRenderer()) {
    makeTex();
}

Textomagic::~Textomagic() {
    TTF_CloseFont(font);
    SDL_DestroyTexture(textTex);
}

void Textomagic::updateBox() {
    textBox.w = draw.w / scale * text.size();
}

void Textomagic::setRect(const SDL_Rect& newDraw) {
    draw = newDraw;
    textBox = newDraw;
    updateBox();
}

void Textomagic::setFont(TTF_Font* newFont) {
    TTF_CloseFont(font);
    font = newFont;
    makeTex();
}

void Textomagic::setScale(const int scaleLen) {
    scale = scaleLen;
}

void Textomagic::setText(const std::string& newText) {
    text = newText;
    if(newText.size() > scale) {
        scale *= 2;
    }
    makeTex();
}

void Textomagic::setColor(const SDL_Color& newColor) {
    color = newColor;
    makeTex();
}

void Textomagic::render() {
    SDL_RenderCopyEx(renderer, textTex, nullptr, &textBox, 0, nullptr, SDL_FLIP_NONE);
}

void Textomagic::makeTex() {
    SDL_Surface *textSurface = TTF_RenderText_Solid(font, text.c_str(), color);
    if (textSurface) {
        SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, textSurface);
        if (texture) {
            if(textTex) {
                SDL_DestroyTexture(textTex);
            }
            //the new texture was created so update the scale rectangle to hold the text
            updateBox();
            textTex = texture;

            SDL_FreeSurface(textSurface);
        } else {
            SDL_DestroyTexture(texture);
        }
    }
}
