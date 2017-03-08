 #ifndef GAMESTATE_MENU_H
#define GAMESTATE_MENU_H
#include "GameState.h"
#include <string>
#include <sstream>
#include <iomanip>
#include "LTexture.h"


class GameStateMenu : public GameState {
public:
    GameStateMenu(Game& g);
    virtual ~GameStateMenu();

    virtual bool load();
    virtual void loop();

    // Frame Display
    std::stringstream frameTimeText;
    TTF_Font* frameFont = nullptr;
    LTexture frameFPSTextTexture;

private:
    virtual void sync() override;
    virtual void handle() override;
    virtual void update(const float delta) override;
    virtual void render() override;
};

#endif
