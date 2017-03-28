#include "Textomagic.h"
#include "Renderer.h"


/**
 * Designer: Isaac Morneau
 * Programmer: Isaac Morneau
 * Date: March 25, 2017
 * Notes:
 *  Guarantees that scaleLen of characters will fit instide the rect draw with equal scaleing
 *  This means that if you put in 20 `l`s depending on the font, it may leave a large portion
 *  of it blank but 20  `W`s will take up the whole space.
 */
Textomagic::Textomagic(const SDL_Rect& draw, TTF_Font* font, const int scaleLen,
        const std::string& initText, const SDL_Color& color)
        :draw(draw), font(font), maxLen(scaleLen), text(initText), color(color), textTex(nullptr),
        textBox(draw), renderer(Renderer::instance().getRenderer()) {
    updateScale();
    makeTex();
}

/**
 * Designer: Isaac Morneau
 * Programmer: Isaac Morneau
 * Date: March 25, 2017
 * Notes:
 *  cleans up allocated memory
 */
Textomagic::~Textomagic() {
    TTF_CloseFont(font);
    SDL_DestroyTexture(textTex);
}
/**
 * Designer: Isaac Morneau
 * Programmer: Isaac Morneau
 * Date: March 25, 2017
 * Notes:
 *  creates a `W` with the current font to test what the max width is
 *  and then sets the internal scale for future rendering to use
 */

void Textomagic::updateScale(){
    //W is the widest latin character for non monospaced fonts on average
    //in this case its being used to calculate the width to height ratio
    SDL_Surface *wSurf = TTF_RenderText_Solid(font, "W", { 0 });
    hScale =  wSurf->h / draw.h;
    wScale = wSurf->w * maxLen / draw.w;
    SDL_FreeSurface(wSurf);
}

/**
 * Designer: Isaac Morneau
 * Programmer: Isaac Morneau
 * Date: March 25, 2017
 * Notes:
 *  recalculats the render destination box based on the texture generated
 */
void Textomagic::updateBox(const int wid) {
    textBox.w = wid / wScale;
}

/**
 * Designer: Isaac Morneau
 * Programmer: Isaac Morneau
 * Date: March 25, 2017
 * Notes:
 *  sets the new place to draw to and rerenders
 */
void Textomagic::setRect(const SDL_Rect& newDraw) {
    draw = newDraw;
    textBox = newDraw;
    makeTex();
}

/**
 * Designer: Isaac Morneau
 * Programmer: Isaac Morneau
 * Date: March 25, 2017
 * Notes:
 *  sets the font to use, cleans up the old font and rerenders
 */
void Textomagic::setFont(TTF_Font* newFont) {
    TTF_CloseFont(font);
    font = newFont;
    updateScale();
    makeTex();
}

/**
 * Designer: Isaac Morneau
 * Programmer: Isaac Morneau
 * Date: March 25, 2017
 * Notes:
 *  force reset the scale length
 */
void Textomagic::setScale(const int scaleLen) {
    maxLen = scaleLen;
}

/**
 * Designer: Isaac Morneau
 * Programmer: Isaac Morneau
 * Date: March 25, 2017
 * Notes:
 *  sets the new text to use and rerenders
 */
void Textomagic::setText(const std::string& newText) {
    text = newText;
    if(static_cast<int>(newText.size()) > maxLen) {
        maxLen *= 1.5;
    }
    makeTex();
}

/**
 * Designer: Isaac Morneau
 * Programmer: Isaac Morneau
 * Date: March 25, 2017
 * Notes:
 *  sets the color to use and rerenders
 */
void Textomagic::setColor(const SDL_Color& newColor) {
    color = newColor;
    makeTex();
}

/**
 * Designer: Isaac Morneau
 * Programmer: Isaac Morneau
 * Date: March 25, 2017
 * Notes:
 *  displays the text via the renderer
 */
void Textomagic::render() {
    SDL_RenderCopyEx(renderer, textTex, nullptr, &textBox, 0, nullptr, SDL_FLIP_NONE);
}

/**
 * Designer: Isaac Morneau
 * Programmer: Isaac Morneau
 * Date: March 25, 2017
 * Notes:
 *  renders the text with the current color, font and draw destination size
 *  recalculates the draw box
 */
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
