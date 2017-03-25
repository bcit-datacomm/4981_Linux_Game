#ifndef PLAYER_H
#define PLAYER_H

#include <SDL2/SDL.h>
#include <string>

#include "../player/Marine.h"
#include "../turrets/Turret.h"
#include "../inventory/Inventory.h"
#include "../game/GameManager.h"

constexpr int PLAYER_PLACE_DISTANCE = 100;

class Player {
public:


    void handleKeyboardInput(const Uint8 *state); // Handles player input with keyboard state
    void handleMouseUpdate(Window& w, float camX, float camY);

    void setControl(Marine& newControl);

    void handleMouseWheelInput(const SDL_Event *e);

    // Added by Mark.C 02/07/2017
    void handlePlacementClick(SDL_Renderer *renderer);

    void handleTempBarricade(SDL_Renderer *renderer);
    void handleTempTurret(SDL_Renderer *renderer);

    Player();
    ~Player();

    //Stays as pointer cause the player gets a marine object after the ctor is called
    Marine *marine = NULL;

private:
    int tempBarricadeID;
    int tempTurretID;
    bool holdingTurret;
    int pickupTick;
    int pickupDelay;
};

#endif
