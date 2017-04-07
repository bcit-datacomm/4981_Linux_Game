#ifndef GAMESTATE_MATCH_H
#define GAMESTATE_MATCH_H

#include <string>
#include <sstream>
#include <iomanip>

#include "../basic/Entity.h"
#include "../game/GameState.h"
#include "../game/GameManager.h"
#include "../game/Level.h"
#include "../player/Player.h"
#include "../buildings/Base.h"
#include "../creeps/Zombie.h"
#include "../sprites/SpriteTypes.h"
#include "../sprites/Renderer.h"
#include "../collision/CollisionHandler.h"
#include "../view/Window.h"
#include "../view/Camera.h"
#include "MatchManager.h"
#include "../game/GameHud.h"

// ticks (ms) in 1 second
static constexpr float TICK_SEC = 1000.0;
static constexpr int STORE_X = 950;
static constexpr int STORE_Y = 700;

static constexpr int DROPZONE_X = 100;
static constexpr int DROPZONE_Y = 100;

class GameStateMatch : public GameState {
public:
    GameStateMatch(Game& g, const int gameWidth, const int gameHeight);
    virtual ~GameStateMatch() = default;

    void updateServ();
    virtual bool load();
    virtual void loop();

private:
    Camera camera;
    MatchManager matchManager;
    GameHud hud;
    SDL_Rect screenRect;

    float storeX;
    float storeY;
    float dropZoneX;
    float dropZoneY;
    float dropZoneSize;

    virtual void sync() override;
    virtual void handle() override;
    virtual void update(const float delta) override;
    virtual void render() override;
};

#endif
