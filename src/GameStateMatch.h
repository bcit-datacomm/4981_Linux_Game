#ifndef GAMESTATE_MATCH_H
#define GAMESTATE_MATCH_H
#include "Entity.h"
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
#include "Window.h"

class GameStateMatch : public GameState {
public:

    virtual bool load();
    virtual void loop();

    GameStateMatch(Game& g, int gameWidth, int gameHeight);

    virtual ~GameStateMatch();



    GameManager* gameManager = NULL;

    // Frame Display
    std::stringstream frameTimeText;
    TTF_Font* frameFont = NULL;
    LTexture frameFPSTextTexture;

private:

    Player player;
    Level level;
    Base base;
    Camera camera;

    virtual void sync();
    virtual void handle();
    virtual void update(const float& delta);
    virtual void render();

};

#endif
