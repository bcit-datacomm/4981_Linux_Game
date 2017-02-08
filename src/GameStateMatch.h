#ifndef GAMESTATE_MATCH_H
#define GAMESTATE_MATCH_H
#include "GameState.h"
#include <string>
#include <sstream>
#include <iomanip>
#include "Player.h"
#include "Level.h"
#include "LTexture.h"
#include "Camera.h"
#include "Base.h"
#include "Zombie.h"
#include "GameManager.h"
#include "CollisionHandler.h"

class GameStateMatch : public GameState {
public:

	virtual bool load();
	virtual void loop();

	GameStateMatch(Game* g) : GameState(g) {};
	virtual ~GameStateMatch();
	
	Player* player = NULL;
	Level* level = NULL;
	Camera* camera = NULL;
	Base* base = NULL;

	GameManager* gameManager = NULL;

	// Frame Display
	std::stringstream frameTimeText;
	TTF_Font* frameFont = NULL;
	LTexture frameFPSTextTexture;
	
private:

	virtual void sync();
	virtual void handle();
	virtual void update(const float& delta);
	virtual void render();
	
};

#endif
