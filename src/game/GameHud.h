#ifndef GAME_HUD_H
#define GAME_HUD_H
#include "../sprites/Renderer.h"
#include "../player/Player.h"
#include "../view/Camera.h"
#include "../../include/Colors.h"

class GameHud {

public:

    GameHud();
    virtual ~GameHud();
    void setRectPosition(SDL_Rect *rect, size_t x, size_t y);
    void setRectSize(SDL_Rect rect, size_t w, size_t h);
    SDL_Rect *getHealthBarBackground();
    SDL_Rect *getHealthBarForeground();
    SDL_Rect *getConsumableSlot();
    SDL_Rect *getEquippedSlot();
    SDL_Rect *getAmmoClipBackground();
    //Need to add funct for weapon slots
    size_t getHealthRgbElement(int index);

    int getOpacity();
    void decrementOpacity(Uint8 amount);
    void setOpacity(Uint8 opacity);
    void setHealthBarColor(float currentHP);
    void renderStaticItems(SDL_Rect screenRect, Player p, Camera c);

private:
    SDL_Rect healthBarBackground;
    SDL_Rect healthBarForeground;
    SDL_Rect consumableSlot;
    SDL_Rect inventorySlot[3];
    SDL_Rect equippedSlot;
    SDL_Rect ammoClipBackground;
    SDL_Rect ammoClipForeground;
    SDL_Rect clipper;

    Uint8 inventorySlotOpacity;
    size_t healthRGB[3];

};

#endif
