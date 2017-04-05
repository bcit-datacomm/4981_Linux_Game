#ifndef GAME_HUD_H
#define GAME_HUD_H
#include "../sprites/Renderer.h"
#include "../player/Player.h"
#include "../view/Camera.h"
#include "../../include/Colors.h"

static constexpr int MAX_HEALTH = 100;
static constexpr int MIN_HEALTH = 0;

static constexpr float EQUIPPED_SLOT_RAT = 0.15;
static constexpr float PADDING_RAT = 0.02;
static constexpr float AMMO_CLIP_BACKROUND_W_RAT = 0.06;
static constexpr float AMMO_CLIP_BACKROUND_H_RAT = 0.20;
static constexpr float HEALTHBAR_BACKROUND_H_RAT = 0.04;
static constexpr float HEALTHBAR_BACKROUND_Y_RAT = 0.01;
static constexpr float HEALTHBAR_FOREGROUND_XY_RAT = 0.1;
static constexpr float HEALTHBAR_FOREGROUND_H_RAT = 0.8;
static constexpr float HEALTHBAR_FOREGROUND_W_RAT = 0.2;
static constexpr float CONSUMABLE_SIZE_RAT = 0.1;
static constexpr float WEAPON_SLOT_WIDTH_RAT = 0.15;
static constexpr float WEAPON_SLOT_HEIGHT_RAT = 0.07;
static constexpr float HALF = 0.5;

class GameHud {

public:

    GameHud();
    virtual ~GameHud() = default;

    size_t getHealthRgbElement(const int index);
    void decrementOpacity(const Uint8 amount);
    void setOpacity(const Uint8 opacity);
    void setHealthBarColor(const float currentHP);
    void renderEquippedWeaponSlot(const SDL_Rect& screenRect, const Player& p);
    void renderClip(const SDL_Rect& screenRect, const Player& p);
    void renderHealthBar(const SDL_Rect& screenRect, const Player& p, const Camera& c);
    void renderConsumable(const SDL_Rect& screenRect, const Player& p);
    void renderWeaponSlots(const SDL_Rect& screenRect, const Player& p);
    void renderInventoryWeapons(SDL_Rect& position, const Player& p, int inventorySlotPosition);
    void renderEquippedWeapon(SDL_Rect& position, const Player& p);

private:
    SDL_Rect healthBarBackground;
    SDL_Rect healthBarForeground;
    SDL_Rect consumableSlot;
    SDL_Rect inventorySlot[3];
    SDL_Rect equippedSlot;
    SDL_Rect ammoClipBackground;
    SDL_Rect ammoClipForeground;
    SDL_Rect ammoClipClipper;

    Uint8 inventorySlotOpacity;
    std::array<size_t, 3> healthRGB;

};

#endif
