 #ifndef GAMESTATE_MENU_H
#define GAMESTATE_MENU_H
#include "GameState.h"
#include <string>
#include <sstream>
#include <iomanip>
#include "LTexture.h"


class GameStateMenu : public GameState {
public:

    virtual bool load();
    virtual void loop();

    GameStateMenu(Game& g);
    virtual ~GameStateMenu();

    // Frame Display
    std::stringstream frameTimeText;
    TTF_Font* frameFont = NULL;
    LTexture frameFPSTextTexture;

private:

    virtual void sync();
    virtual void handle();
    virtual void update(const float& delta);
    virtual void render();

};

#endif
