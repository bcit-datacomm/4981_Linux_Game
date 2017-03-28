#include "Textomagic.h"
#include "Renderer.h"

Textomagic::Textomagic(const SDL_Rect& draw, TTF_Font* font, const int scaleLen,
        const std::string& initText, const SDL_Color& color)
        :draw(draw), font(font), maxLen(scaleLen), text(initText), color(color), textTex(nullptr),
        textBox(draw), renderer(Renderer::instance().getRenderer()) {
    updateScale();
    makeTex();
}

Textomagic::~Textomagic() {
    TTF_CloseFont(font);
    SDL_DestroyTexture(textTex);
}

void Textomagic::updateScale(){
    //W is the widest latin character for non monospaced fonts on average
    //in this case its being used to calculate the width to height ratio
    SDL_Surface *wSurf = TTF_RenderText_Solid(font, "W", { 0 });
    hScale =  wSurf->h / draw.h;
    wScale = wSurf->w * maxLen / draw.w;
    SDL_FreeSurface(wSurf);
}

void Textomagic::updateBox(const int wid) {
    textBox.w = wid / wScale;
}

void Textomagic::setRect(const SDL_Rect& newDraw) {
    draw = newDraw;
    textBox = newDraw;
    makeTex();
}

void Textomagic::setFont(TTF_Font* newFont) {
    TTF_CloseFont(font);
    font = newFont;
    updateScale();
    makeTex();
}

void Textomagic::setScale(const int scaleLen) {
    maxLen = scaleLen;
}

void Textomagic::setText(const std::string& newText) {
    text = newText;
    if(static_cast<int>(newText.size()) > maxLen) {
        maxLen *= 1.5;
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
            updateBox(textSurface->w);
            textTex = texture;

            SDL_FreeSurface(textSurface);
        } else {
            SDL_DestroyTexture(texture);
        }
    }
}
