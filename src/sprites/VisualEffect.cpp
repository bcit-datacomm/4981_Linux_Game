#include "VisualEffect.h"
#include "Renderer.h"
#include <SDL2/SDL.h>

VisualEffect VisualEffect::sInstance;

/**
 * Developer: Isaac Morneau
 * Designer: Isaac Morneau
 * Date: March 25, 2017
 * Notes:
 * sets the ids to 0 the rest can be default initialized
 */
VisualEffect::VisualEffect():preLineId(0), preRectId(0), preTexId(0), postLineId(0),
    postRectId(0), postTexId(0) {}

/**
 * Developer: Isaac Morneau
 * Designer: Isaac Morneau
 * Date: March 25, 2017
 * Notes:
 * helper function to calculate the screen rect based on the camera
 */
inline constexpr SDL_Rect relative(const SDL_Rect& dest, const SDL_Rect& camera){
    return {dest.x - camera.x, dest.y - camera.y, dest.w, dest.h};
}

/**
 * Developer: Isaac Morneau
 * Designer: Isaac Morneau
 * Date: March 25, 2017
 * Notes:
 * render and remove timed out effects
 */
void VisualEffect::renderPreEntity(const SDL_Rect &camera) {
    auto& renderer = Renderer::instance();
    SDL_Renderer *rend = renderer.getRenderer();
    for (auto p = preLines.begin(); p != preLines.end();) {
        if (--p->second.dur > 0) {
            SDL_SetRenderDrawColor(rend, p->second.r, p->second.g, p->second.b, p->second.a);
            SDL_RenderDrawLine(rend, p->second.x - camera.x, p->second.y - camera.y,
                   p->second.ex - camera.x, p->second.ey - camera.y);

            SDL_RenderDrawLine(rend, p->second.x - camera.x + 1, p->second.y - camera.y + 1,
                p->second.ex - camera.x + 1, p->second.ey - camera.y + 1);

            SDL_RenderDrawLine(rend, p->second.x - camera.x - 1, p->second.y - camera.y - 1,
                p->second.ex - camera.x - 1, p->second.ey - camera.y - 1);
            ++p;
        } else {
            p = preLines.erase(p);
        }
    }
    for (auto p = preRects.begin(); p != preRects.end();) {
        if (--p->second.dur > 0) {
            SDL_SetRenderDrawColor(rend, p->second.r, p->second.g, p->second.b, p->second.a);
            const SDL_Rect temp = relative(p->second.s, camera);
            SDL_RenderDrawRect(rend, &temp);
            ++p;
        } else {
            p = preRects.erase(p);
        }
    }
    for (auto p = preTex.begin(); p != preTex.end();) {
        if (--p->second.dur > 0) {
            const SDL_Rect temp = relative(p->second.dest, camera);
            renderer.render(temp, p->second.tex, p->second.src);
            ++p;
        } else {
            p = preTex.erase(p);
        }
    }
}

/**
 * Developer: Isaac Morneau
 * Designer: Isaac Morneau
 * Date: March 25, 2017
 * Notes:
 * render and remove timed out effects
 */
void VisualEffect::renderPostEntity(const SDL_Rect &camera) {
    auto& renderer = Renderer::instance();
    SDL_Renderer *rend = renderer.getRenderer();
    for (auto p = postLines.begin(); p != postLines.end();) {
        if (--p->second.dur > 0) {
            SDL_SetRenderDrawColor(rend, p->second.r, p->second.g, p->second.b, p->second.a);
            SDL_RenderDrawLine(rend, p->second.x - camera.x, p->second.y - camera.y,
                    p->second.ex - camera.x, p->second.ey - camera.y);
            ++p;
        } else {
            p = postLines.erase(p);
        }
    }
    for (auto p = postRects.begin(); p != postRects.end();) {
        if (--p->second.dur > 0) {
            SDL_SetRenderDrawColor(rend, p->second.r, p->second.g, p->second.b, p->second.a);
            const SDL_Rect temp = relative(p->second.s, camera);
            SDL_RenderDrawRect(rend, &temp);
            ++p;
        } else {
            p = postRects.erase(p);
        }
    }
    for (auto p = postTex.begin(); p != postTex.end();) {
        if (--p->second.dur > 0) {
            const SDL_Rect temp = relative(p->second.dest, camera);
            renderer.render(temp, p->second.tex, p->second.src);
            ++p;
        } else {
            p = postTex.erase(p);
        }
    }
}


/**
 * Developer: Isaac Morneau
 * Designer: Isaac Morneau
 * Date: March 25, 2017
 * Notes:
 * add a line effect
 */
int VisualEffect::addPreLine (const int dur, const int startx, const int starty, const int endx,
        const int endy, const Uint8 r, const Uint8 g, const Uint8 b, const Uint8 a) {
    preLines[++preLineId] = {dur, startx, starty, endx, endy, r, g, b, a};
    return preLineId;
}

/**
 * Developer: Isaac Morneau
 * Designer: Isaac Morneau
 * Date: March 25, 2017
 * Notes:
 * add a line effect
 */
int VisualEffect::addPostLine(const int dur, const int startx, const int starty, const int endx,
        const int endy, const Uint8 r, const Uint8 g, const Uint8 b, const Uint8 a) {
    postLines[++postLineId] = {dur, startx, starty, endx, endy, r, g, b, a};
    return postLineId;
}

/**
 * Developer: Isaac Morneau
 * Designer: Isaac Morneau
 * Date: March 25, 2017
 * Notes:
 * add a rect effect
 */
int VisualEffect::addPreRect(const int dur, const SDL_Rect &dest, const Uint8 r,
        const Uint8 g, const Uint8 b, const Uint8 a) {
    preRects[++preRectId] = {dur, dest, r, g, b, a};
    return preRectId;
}

/**
 * Developer: Isaac Morneau
 * Designer: Isaac Morneau
 * Date: March 25, 2017
 * Notes:
 * add a rect effect
 */
int VisualEffect::addPostRect(const int dur, const SDL_Rect &dest, const Uint8 r,
        const Uint8 g, const Uint8 b, const Uint8 a) {
    postRects[++postRectId] = {dur, dest, r, g, b, a};
    return postRectId;
}

/**
 * Developer: Isaac Morneau
 * Designer: Isaac Morneau
 * Date: March 25, 2017
 * Notes:
 * add a texture effect
 */
int VisualEffect::addPreTex(const int dur, const SDL_Rect &src, const SDL_Rect &dest, const TEXTURES tex) {
    preTex[++preTexId] = {dur, tex, src, dest};
    return preTexId;
}

/**
 * Developer: Isaac Morneau
 * Designer: Isaac Morneau
 * Date: March 25, 2017
 * Notes:
 * add a texture effect
 */
int VisualEffect::addPostTex(const int dur, const SDL_Rect &src, const SDL_Rect &dest, const TEXTURES tex) {
    postTex[++postTexId] = {dur, tex, src, dest};
    return postTexId;
}

/**
 * Developer: Isaac Morneau
 * Designer: Isaac Morneau
 * Date: March 25, 2017
 * Notes:
 * cancel effect with given id
 */
void VisualEffect::removePreLine(const int id) {
    preLines.erase(id);
}

/**
 * Developer: Isaac Morneau
 * Designer: Isaac Morneau
 * Date: March 25, 2017
 * Notes:
 * cancel effect with given id
 */
void VisualEffect::removePreRect(const int id) {
    preRects.erase(id);
}

/**
 * Developer: Isaac Morneau
 * Designer: Isaac Morneau
 * Date: March 25, 2017
 * Notes:
 * cancel effect with given id
 */
void VisualEffect::removePreTex(const int id) {
    preTex.erase(id);
}

/**
 * Developer: Isaac Morneau
 * Designer: Isaac Morneau
 * Date: March 25, 2017
 * Notes:
 * cancel effect with given id
 */
void VisualEffect::removePostLine(const int id) {
    postLines.erase(id);
}

/**
 * Developer: Isaac Morneau
 * Designer: Isaac Morneau
 * Date: March 25, 2017
 * Notes:
 * cancel effect with given id
 */
void VisualEffect::removePostRect(const int id) {
    postRects.erase(id);
}

/**
 * Developer: Isaac Morneau
 * Designer: Isaac Morneau
 * Date: March 25, 2017
 * Notes:
 * cancel effect with given id
 */
void VisualEffect::removePostTex(const int id) {
    postTex.erase(id);
}
