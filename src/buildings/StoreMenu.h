#ifndef STORE_MENU_H
#define STORE_MENU_H
#include "../sprites/Renderer.h"
#include "../sprites/SpriteTypes.h"
#include "../player/Player.h"
#include "../view/Camera.h"
#include "../../include/Colors.h"
#include "../game/GameHashMap.h"
#include "../sprites/Textomagic.h"

static constexpr int TOTAL_SLOTS = 9;
//static constexpr int MIN_HEALTH = 0;



class StoreMenu {

public:
    StoreMenu(const SDL_Rect d, GameHashMap<TEXTURES, int> i);
    virtual ~StoreMenu() = default;

    void renderBackground();
    void renderSlots(const int num);
    void setSizes();
    int getClicked(const float x, const float y);
    int checkSlot(SDL_Rect& s, float x, float y);

private:
    SDL_Rect screen;
    int type = 1;
    GameHashMap<TEXTURES, int> Items;
    Uint8 inventorySlotOpacity;
    SDL_Rect background;
    SDL_Rect slot[TOTAL_SLOTS];
    SDL_Rect selectedSlot;
};

#endif
