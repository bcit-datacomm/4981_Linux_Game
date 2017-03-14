#ifndef PLAYER_H
#define PLAYER_H
#include <string>
#include "../sprites/LTexture.h"
#include "../player/Marine.h"
#include "../turrets/Turret.h"
#include "../inventory/Inventory.h"
#include <SDL2/SDL.h>
#include "../game/GameManager.h"

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
};

#endif