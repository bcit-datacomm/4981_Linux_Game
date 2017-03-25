#ifndef VISUALEFFECT_H
#define VISUALEFFECT_H

#include <SDL2/SDL.h>
#include <unordered_map>
#include "SpriteTypes.h"


class VisualEffect {
    public:
        static VisualEffect& instance(){
            return sInstance;
        }

        void renderPreEntity(const SDL_Rect &camera);
        
        void renderPostEntity(const SDL_Rect &camera);

        int addPreLine(const int dur, const int startx, const int starty, const int endx,
            const int endy, const Uint8 r = 0, const Uint8 g = 0, const Uint8 b = 0, const Uint8 a = 255);

        int addPostLine(const int dur, const int startx, const int starty, const int endx,
            const int endy, const Uint8 r = 0, const Uint8 g = 0, const Uint8 b = 0, const Uint8 a = 255);

        int addPreRect(const int dur, const SDL_Rect &dest, const Uint8 r = 0, const Uint8 g = 0,
            const Uint8 b = 0, const Uint8 a = 255);

        int addPostRect(const int dur, const SDL_Rect &dest, const Uint8 r = 0, const Uint8 g = 0,
            const Uint8 b = 0, const Uint8 a = 255);

        int addPreTex(const int dur, const SDL_Rect &src, const SDL_Rect &dest, const TEXTURES tex);

        int addPostTex(const int dur, const SDL_Rect &src, const SDL_Rect &dest, const TEXTURES tex);
        



        void removePreLine(const int id);
        
        void removePreRect(const int id);
        
        void removePreTex(const int id);

        void removePostLine(const int id);

        void removePostRect(const int id);

        void removePostTex(const int id);


    private:

        VisualEffect();
        ~VisualEffect() = default;

        static VisualEffect sInstance;

        //internal classes to manage the different types
        struct Line {
            int dur;
            
            int x, y;
            int ex, ey;

            Uint8 r;
            Uint8 g;
            Uint8 b;
            Uint8 a;
        };

        struct Rect {
            int dur;
            
            SDL_Rect s;
            
            Uint8 r;
            Uint8 g;
            Uint8 b;
            Uint8 a;
        };

        struct Tex {
            int dur;

            TEXTURES tex;

            SDL_Rect src, dest;
        };

        int preLineId;
        int preRectId;
        int preTexId;

        int postLineId;
        int postRectId;
        int postTexId;
        
        std::unordered_map<int, Line> preLines;
        std::unordered_map<int, Rect> preRects;
        std::unordered_map<int, Tex> preTex;

        std::unordered_map<int, Line> postLines;
        std::unordered_map<int, Rect> postRects;
        std::unordered_map<int, Tex> postTex;
};

#endif
