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
#include "../basic/LTimer.h"

// ticks (ms) in 1 second
static constexpr float TICK_SEC = 1000;

class GameStateMatch : public GameState {
public:
    GameStateMatch(Game& g, int gameWidth, int gameHeight);
    virtual ~GameStateMatch() = default;

    virtual bool load();
    virtual void loop();

    GameManager* gameManager = nullptr;

    // Frame Display
    std::stringstream frameTimeText;
    //TTF_Font* frameFont = nullptr;

private:
    Player player;
    Base base;
    Camera camera;

    virtual void sync() override;
    virtual void handle() override;
    virtual void update(const float delta) override;
    virtual void render() override;

};

#endif
