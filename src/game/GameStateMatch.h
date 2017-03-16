#ifndef GAMESTATE_MATCH_H
#define GAMESTATE_MATCH_H
#include "../basic/Entity.h"
#include "GameState.h"
#include <string>
#include <sstream>
#include <iomanip>
#include "../player/Player.h"
#include "Level.h"
#include "../sprites/LTexture.h"
#include "../view/Camera.h"
#include "../buildings/Base.h"
#include "../creeps/Zombie.h"
#include "GameManager.h"
#include "../collision/CollisionHandler.h"
#include "../view/Window.h"
#include "../sprites/SpriteManager.h"

// ticks (ms) in 1 second
static constexpr float TICK_SEC = 1000;

// font size
static constexpr int FONT_SIZE = 28;

class GameStateMatch : public GameState {
public:
    GameStateMatch(Game& g, int gameWidth, int gameHeight);
    virtual ~GameStateMatch();

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

    virtual void sync() override;
    virtual void handle() override;
    virtual void update(const float delta) override;
    virtual void render() override;

};

#endif
