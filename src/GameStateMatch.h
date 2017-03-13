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
    GameStateMatch(Game& g, int gameWidth, int gameHeight);
    virtual ~GameStateMatch();

    void updateServ();
    virtual bool load();
    virtual void loop();

    GameManager* gameManager = nullptr;

    // Frame Display
    std::stringstream frameTimeText;
    TTF_Font* frameFont = nullptr;
    LTexture frameFPSTextTexture;


private:
    Player player;
    Level level;
    Base base;
    Camera camera;

    static bool gameStart;
    virtual void sync() override;
    virtual void handle() override;
    virtual void update(const float delta) override;
    virtual void render() override;
};

#endif
