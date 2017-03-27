#ifndef GAMESTATE_MENU_H
#define GAMESTATE_MENU_H

#include "GameState.h"
#include <string>
#include <sstream>
#include <iomanip>

#include "../game/Level.h"
#include "../view/Camera.h"

static constexpr size_t maxLength = 15;

//Position of color in the fontColor array
static constexpr size_t WHITE = 0;
static constexpr size_t GREEN = 1;
static constexpr size_t BLACK = 2;
static constexpr size_t RED = 3;

static constexpr int ZERO = 0;
static constexpr int FONT_SIZE = 30;

class GameStateMenu : public GameState {
public:

    GameStateMenu(Game& g);

    virtual ~GameStateMenu();

    virtual bool load();
    virtual void loop();

private:

    virtual void sync() override;
    virtual void handle() override;
    virtual void update(const float delta) override;
    void positionElements();
    virtual void render() override;

    TTF_Font* headingFont;
    TTF_Font* textboxFont;
    TTF_Font* menuFont;

    SDL_Rect usernameTextBox;
    SDL_Rect hostIPTextBox;

    SDL_Rect joinRect;
    SDL_Rect optionsRect;

    SDL_Rect screenRect;

    bool joinSelected;
    bool optionsSelected;
    bool hostIPSelected;
    bool usernameSelected;
    std::string hostIpDefaultText;
    std::string usernameDefaultText;
    std::string hostIpTextInput;
    std::string usernameTextInput;


};

#endif
