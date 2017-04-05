#ifndef GAMESTATE_MENU_H
#define GAMESTATE_MENU_H

#include "Game.h"
#include "GameState.h"
#include <string>

#include "../game/Level.h"
#include "../view/Camera.h"
#include "../sprites/Textomagic.h"
#include "../audio/AudioManager.h"

static constexpr size_t maxLength = 15;
static constexpr int FONT_SIZE = 256;
static constexpr int JOIN_LEN = 4;

static const std::string HOST_DEFAULT("Hostname");
static const std::string USER_DEFAULT("Username");

static constexpr float HOST_X_RAT = 1.0 / 3;
static constexpr float HOST_Y_RAT = 1.0 / 2;
static constexpr float HOST_W_RAT = 1.0 / 3;
static constexpr float HOST_H_RAT = 1.0 / 14;

static constexpr float USER_X_RAT = 1.0 / 3;
static constexpr float USER_Y_PAD = 50;
static constexpr float USER_W_RAT = 1.0 / 3;
static constexpr float USER_H_RAT = 1.0 / 14;

static constexpr float JOIN_X_RAT = 4.0 / 12;
static constexpr float JOIN_Y_PAD = 50;
static constexpr float JOIN_W_RAT = 1.0 / 3;
static constexpr float JOIN_H_RAT = 1.0 / 7;

//Color Data
static constexpr SDL_Color WHITE = {255, 255, 255, 255};
static constexpr SDL_Color RED = {143, 63, 61, 255};
static constexpr SDL_Color BLACK = {0, 0, 0, 255};
static constexpr SDL_Color GREEN = {54, 96, 81, 255};



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
