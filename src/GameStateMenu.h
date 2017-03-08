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

    GameStateMenu(Game* g) : GameState(g) {};
	virtual ~GameStateMenu();

	virtual bool load();
	virtual void loop();

	Level* level    = NULL;
	Camera* camera  = NULL;

	TTF_Font* headingFont   = NULL;
	TTF_Font* textboxFont   = NULL;
	TTF_Font* menuFont      = NULL;
	LTexture frameFPSTextTexture;

private:

	virtual void sync();
	virtual void handle();
	virtual void update(const float& delta);
	virtual void render();

	void positionElements();
	void renderText(LTexture fontTexture, const char* text, SDL_Color color, TTF_Font* font, int x, int y);

	static const int 	NUM_MENU_ITEMS 				= 2;
	static const int 	JOIN         				= 0;
	static const int 	OPTIONS        				= 1;
	const char* 		menuItems[NUM_MENU_ITEMS] 	= {"join", "options"};
	bool 				selected[NUM_MENU_ITEMS] 	= {false,false};
	SDL_Rect 			pos[NUM_MENU_ITEMS];
	LTexture 			menuTextTextures[NUM_MENU_ITEMS];

	static const int 	NUM_TEXT_FIELDS				        = 2;
	static const int 	IP         				            = 0;
	static const int 	USERNAME        				    = 1;
	const char* 		textboxTitles[NUM_TEXT_FIELDS]      = {"Server IP:", "Username:"};
	bool 				activeTextbox[NUM_MENU_ITEMS] 	    = {false,false};
	SDL_Rect 			textboxTitlePos[NUM_TEXT_FIELDS];
	SDL_Rect 			textboxPos[NUM_TEXT_FIELDS];
	LTexture 			textboxTitleTextures[NUM_TEXT_FIELDS];

	std::string         textInput[NUM_TEXT_FIELDS]  = {"IP Address:", "Username"};
	const size_t        maxLength                   = 15;
	LTexture 			textboxTextures[NUM_TEXT_FIELDS];

	SDL_Color 			fontColors[3]   = {{255,255,255}, {54,96,81}, {0,0,0}};
	const size_t        WHITE           = 0;
	const size_t        GREEN           = 1;
	const size_t        BLACK           = 2;
	const size_t        OPAQUE          = 255;
	const size_t        WHITE_RGB[3]    = {255, 255, 255};
	const size_t        YELLOW_RGB[3]   = {148, 139, 77};
};

#endif
