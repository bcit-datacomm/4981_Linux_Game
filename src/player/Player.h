#ifndef PLAYER_H
#define PLAYER_H
#include <string>
#include <memory>
#include "../sprites/LTexture.h"
#include "../player/Marine.h"
#include "../turrets/Turret.h"
#include "../inventory/Inventory.h"
#include <SDL2/SDL.h>
#include "../game/GameManager.h"

static constexpr int PLAYER_PLACE_DISTANCE = 100;

class Player {
public:
    Player(const int32_t& id);
    ~Player() = default;

    void handleKeyboardInput(const Uint8 *state); // Handles player input with keyboard state
    void handleMouseUpdate(Window& w, const float camX, const float camY);

    void handleMouseWheelInput(const SDL_Event *e);

    // Added by Mark.C 02/07/2017
    void handlePlacementClick(SDL_Renderer *renderer);

    void handleTempBarricade(SDL_Renderer *renderer);
    void handleTempTurret(SDL_Renderer *renderer);

    Marine& getMarine() const {return marine;}

private:
    int tempBarricadeID;
    int tempTurretID;
    Marine& marine;
};

#endif
