 #ifndef GAMESTATE_MENU_H
#define GAMESTATE_MENU_H
#include "GameState.h"
#include <string>
#include <sstream>
#include <iomanip>
#include "Level.h"
#include "Camera.h"
#include "LTexture.h"


class GameStateMenu : public GameState {
public:

	virtual bool load();
	virtual void loop();

	GameStateMenu(Game* g) : GameState(g) {};
	virtual ~GameStateMenu();

	Level* level = NULL;
	Camera* camera = NULL;
	
	// Frame Display
	std::stringstream frameTimeText;
	TTF_Font* frameFont = NULL;
	TTF_Font* menuFont = NULL;
	LTexture frameFPSTextTexture;
	
private:

	virtual void sync();
	virtual void handle();
	virtual void update(const float& delta);
	virtual void render();

	static const int NUM_MENU_ITEMS = 2;
	const char* menuItems[NUM_MENU_ITEMS] = {"join", "options"};
	bool selected[NUM_MENU_ITEMS] = {false,false};
	SDL_Color fontColors[2] = {{255,255,255}, {54,96,81}};	
	SDL_Rect pos[NUM_MENU_ITEMS];
	LTexture menuTextTextures[NUM_MENU_ITEMS];
};

#endif
