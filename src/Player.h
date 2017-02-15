#ifndef PLAYER_H
#define PLAYER_H
#include <string>
#include "LTexture.h"
#include "Marine.h"
#include "Turret.h"
#include "Inventory.h"
#include <SDL2/SDL.h>
#include "GameManager.h"

class Player {
public:

	void handleKeyboardInput(const Uint8 *state); // Handles player input with keyboard state
	void handleMouseUpdate(Window* w, float camX, float camY);

	void setControl(Marine* newControl);

	void handleMouseWheelInput(const SDL_Event *e);

    // Added by Mark.C 02/07/2017
    void handlePlacementClick(SDL_Renderer *renderer);

    // determines whether turret can be placed
    void turretPlaceCheck(float x, float y, CollisionHandler* collisionHandler, Turret* dumbTurret,
                          unsigned int tid);

	void placeBarricade(SDL_Renderer *renderer);

	Player();
	~Player();

	Marine* marine = NULL;
	Barricade* tempBarricade = NULL;
private:


};

#endif
