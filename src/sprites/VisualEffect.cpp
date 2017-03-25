#include "VisualEffect.h"
#include "Renderer.h"
#include <SDL2/SDL.h>

VisualEffect VisualEffect::sInstance;

VisualEffect::VisualEffect():preLineId(0), preRectId(0), preTexId(0), postLineId(0),
    postRectId(0), postTexId(0) { }

inline const SDL_Rect relative(const SDL_Rect& dest, const SDL_Rect& camera){
    return {dest.x - camera.x, dest.y - camera.y, dest.w, dest.h};
}

void VisualEffect::renderPreEntity(const SDL_Rect &camera) {
    auto& renderer = Renderer::instance();
    SDL_Renderer *rend = renderer.getRenderer();
    for (auto& p : preLines) {
        if (--p.second.dur > 0) {
            SDL_SetRenderDrawColor(rend, p.second.r, p.second.g, p.second.b, p.second.a);
            SDL_RenderDrawLine(rend, p.second.x - camera.x, p.second.y - camera.y,
                    p.second.ex - camera.x, p.second.ey - camera.y);
        }
    }
    for (auto& p : preRects) {
        if (--p.second.dur > 0) {
            SDL_SetRenderDrawColor(rend, p.second.r, p.second.g, p.second.b, p.second.a);
            const SDL_Rect temp = relative(p.second.s, camera);
            SDL_RenderDrawRect(rend, &temp);
        }
    } 
    for (auto& p : preTex) {
        if (--p.second.dur > 0) {
            const SDL_Rect temp = relative(p.second.dest, camera);
            renderer.render(temp, p.second.tex, p.second.src);
        }
    } 
}

void VisualEffect::renderPostEntity(const SDL_Rect &camera) {
    auto& renderer = Renderer::instance();
    SDL_Renderer *rend = renderer.getRenderer();
    for (auto& p : postLines) {
        if (--p.second.dur > 0) {
            SDL_SetRenderDrawColor(rend, p.second.r, p.second.g, p.second.b, p.second.a);
            SDL_RenderDrawLine(rend, p.second.x - camera.x, p.second.y - camera.y,
                    p.second.ex - camera.x, p.second.ey - camera.y);
        }
    }
    for (auto& p : postRects) {
        if (--p.second.dur > 0) {
            SDL_SetRenderDrawColor(rend, p.second.r, p.second.g, p.second.b, p.second.a);
            const SDL_Rect temp = relative(p.second.s, camera);
            SDL_RenderDrawRect(rend, &temp);
        }
    } 
    for (auto& p : postTex) {
        if (--p.second.dur > 0) {
            const SDL_Rect temp = relative(p.second.dest, camera);
            renderer.render(temp, p.second.tex, p.second.src);
        }
    } 
}


int VisualEffect::addPreLine (const int dur, const int startx, const int starty, const int endx,
        const int endy, const Uint8 r, const Uint8 g, const Uint8 b, const Uint8 a) {
    preLines[++preLineId] = {dur, startx, starty, endx, endy, r, g, b, a};
    return preLineId;
}

int VisualEffect::addPostLine(const int dur, const int startx, const int starty, const int endx,
        const int endy, const Uint8 r, const Uint8 g, const Uint8 b, const Uint8 a) {
    preLines[++preLineId] = {dur, startx, starty, endx, endy, r, g, b, a};
    return postLineId;
}

int VisualEffect::addPreRect(const int dur, const SDL_Rect &dest, const Uint8 r,
        const Uint8 g, const Uint8 b, const Uint8 a) {
    preRects[++preRectId] = {dur, dest, r, g, b, a};
    return preRectId;
}

int VisualEffect::addPostRect(const int dur, const SDL_Rect &dest, const Uint8 r,
        const Uint8 g, const Uint8 b, const Uint8 a) {
    postRects[++postRectId] = {dur, dest, r, g, b, a};
    return postRectId;
}

int VisualEffect::addPreTex(const int dur, const SDL_Rect &src, const SDL_Rect &dest, const TEXTURES tex) {
    preTex[++preTexId] = {dur, tex, src, dest};
    return preTexId;
}

int VisualEffect::addPostTex(const int dur, const SDL_Rect &src, const SDL_Rect &dest, const TEXTURES tex) {
    postTex[++postTexId] = {dur, tex, src, dest};
    return postTexId;
}

void VisualEffect::removePreLine(const int id) {
    preLines.erase(id);
}

void VisualEffect::removePreRect(const int id) {
    preRects.erase(id);
}

void VisualEffect::removePreTex(const int id) {
    preTex.erase(id);
}

void VisualEffect::removePostLine(const int id) {
    postLines.erase(id);
}

void VisualEffect::removePostRect(const int id) {
    postRects.erase(id);
}

void VisualEffect::removePostTex(const int id) {
    postTex.erase(id);
}

