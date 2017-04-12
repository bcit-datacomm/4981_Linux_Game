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

constexpr int TOTAL_SLOTS = 9;
constexpr int TECH_SLOTS = 2;
constexpr int SLOT_0_X = 16;
constexpr int BACKGROUND_H_PADDING = 50;
constexpr int BACKGROUND_V_PADDING = 400;
constexpr int BACKGROUND_WIDTH = 4;

constexpr double BACKGROUND_HEIGHT = .2;
constexpr double WEAPONS_HEIGHT = .4;

constexpr int SLOT_SIZE = 4;
constexpr int SLOT_X = 16;
constexpr int V_SPACE = 1.2;

static constexpr int B_SRC_X = 15;
static constexpr int B_SRC_Y = 478;
static constexpr int B_SRC_W = 122;
static constexpr int B_SRC_H = 83;
class StoreMenu {

public:
    StoreMenu(const SDL_Rect d, GameHashMap<TEXTURES, int> i, int t);
    virtual ~StoreMenu() = default;

    void renderBackground();
    void renderSlots(const int num);
    void setSizes();
    int getClicked(const float x, const float y);
    bool checkSlot(const SDL_Rect& s, const float x, const float y);
    void createWeaponStoreMenu();
    void createTechStoreMenu();
    void createHealthStoreMenu();
    void renderText();
    void creatWeaponStoreText();
    void creatTechStoreText();
    void creatHealthStoreText();
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
