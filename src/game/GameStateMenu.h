#ifndef GAMESTATE_MENU_H
#define GAMESTATE_MENU_H

#include "Game.h"
#include "GameState.h"
#include <string>

#include "../game/Level.h"
#include "../view/Camera.h"
#include "../sprites/Textomagic.h"

static constexpr size_t maxLength = 15;
static constexpr int FONT_SIZE = 100;

static const std::string HOST_DEFAULT("Hostname");
static const std::string USER_DEFAULT("Username");

class GameStateMenu : public GameState {
public:
    GameStateMenu(Game& g);
    virtual ~GameStateMenu() = default;

    virtual bool load();
    virtual void loop();

private:
    virtual void sync() override;
    virtual void handle() override;
    virtual void update(const float delta) override;
    void positionElements();
    virtual void render() override;

    SDL_Rect screenRect;

    SDL_Rect hostRect;
    SDL_Rect userRect;
    SDL_Rect joinRect;

    bool joinSelected;
    bool hostIPSelected;
    bool usernameSelected;

    std::string hostInput;
    std::string userInput;
    
    Textomagic hostMagic;
    Textomagic userMagic;
    Textomagic joinMagic;

};

#endif
