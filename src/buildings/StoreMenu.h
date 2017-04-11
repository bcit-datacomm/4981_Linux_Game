#ifndef STORE_MENU_H
#define STORE_MENU_H
#include "../sprites/Renderer.h"
#include "../sprites/SpriteTypes.h"
#include "../player/Player.h"
#include "../view/Camera.h"
#include "../../include/Colors.h"
#include "../game/GameHashMap.h"
#include "../sprites/Textomagic.h"

#include <SDL2/SDL_ttf.h>

static constexpr int TOTAL_SLOTS = 9;
//static constexpr int MIN_HEALTH = 0;



class StoreMenu {

public:
    StoreMenu(const SDL_Rect d, GameHashMap<TEXTURES, int> i, int t);
    virtual ~StoreMenu() = default;

    void renderBackground();
    void renderSlots(const int num);
    void setSizes();
    int getClicked(const float x, const float y);
    bool checkSlot(SDL_Rect& s, float x, float y);
    void createWeaponStoreMenu();
    void createTechStoreMenu();
    void createHealthStoreMenu();
    void renderText();
    void creatWeaponStoreText();
    void StoreMenu::creatWeaponStoreText();
    void StoreMenu::creatStoreText();
    void StoreMenu::creatHealthStoreText();
private:
    SDL_Rect screen;
    GameHashMap<TEXTURES, int> Items;
    int type;
    Uint8 inventorySlotOpacity;
    SDL_Rect background;
    std::array<SDL_Rect, TOTAL_SLOTS> slot;
    SDL_Rect selectedSlot;
    TTF_Font *font;
};

#endif
