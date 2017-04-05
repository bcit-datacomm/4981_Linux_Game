#ifndef GAMESTATE_MATCH_H
#define GAMESTATE_MATCH_H

#include <string>
#include <sstream>
#include <iomanip>

#include "../basic/Entity.h"
#include "../game/GameState.h"
#include "../player/Player.h"
#include "../game/Level.h"
#include "../view/Camera.h"
#include "../buildings/Base.h"
#include "../creeps/Zombie.h"
#include "../game/GameManager.h"
#include "../sprites/SpriteTypes.h"
#include "../sprites/Renderer.h"
#include "../collision/CollisionHandler.h"
#include "../view/Window.h"

// ticks (ms) in 1 second
static constexpr float TICK_SEC = 1000.0;
static constexpr int STORE_X = 950;
static constexpr int STORE_Y = 700;

static constexpr int DROPZONE_X = 100;
static constexpr int DROPZONE_Y = 100;
static constexpr int DROPZONE_SIZE = 4;

static constexpr int BASE_SRC_X = 82;
static constexpr int BASE_SRC_Y = 44;
static constexpr int BASE_SRC_W = 1012;
static constexpr int BASE_SRC_H = 1050;




class GameStateMatch : public GameState {
public:
    GameStateMatch(Game& g, const int gameWidth, const int gameHeight);
    virtual ~GameStateMatch() = default;

    virtual bool load();
    virtual void loop();

private:
    Player player;
    Base base;
    Camera camera;
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
