#ifndef PLAYER_H
#define PLAYER_H
#include <string>
#include "LTexture.h"
#include "Marine.h"
#include "Inventory.h"
#include <SDL2/SDL.h>

class Player {
public:
	
	void handleKeyboardInput(const Uint8 *state); // Handles player input with keyboard state
	void handleMouseInput(Window* w);
	
	void setControl(Marine* newControl);

	Player();
	~Player();

	Marine* marine = NULL;

private:


};

#endif
