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
<<<<<<< HEAD
#include "MatchManager.h"
=======
#include "../game/GameHud.h"
>>>>>>> 33e3c5bdd29b44e599921878cb1d547c41060c99

// ticks (ms) in 1 second
static constexpr float TICK_SEC = 1000.0;

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
<<<<<<< HEAD
    MatchManager matchManager;
=======
    GameHud hud;

    SDL_Rect screenRect;
>>>>>>> 33e3c5bdd29b44e599921878cb1d547c41060c99
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
