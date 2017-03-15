 #ifndef GAMESTATE_MENU_H
#define GAMESTATE_MENU_H
#include "GameState.h"
#include <string>
#include <sstream>
#include <iomanip>
#include "../game/Level.h"
#include "../view/Camera.h"
#include "../sprites/LTexture.h"

static constexpr size_t NUM_MENU_ITEMS = 2;
static constexpr size_t JOIN = 0;
static constexpr size_t OPTIONS = 1;
static constexpr size_t NUM_TEXT_FIELDS = 2;
static constexpr size_t IP = 0;
static constexpr size_t USERNAME = 1;
static constexpr size_t maxLength = 15;

//Color Data
static constexpr SDL_Color SDL_WHITE_RGB = {255, 255, 255};
static constexpr SDL_Color SDL_RED_RGB = {143, 63, 61};
static constexpr SDL_Color SDL_BLACK_RGB = {0, 0, 0};
static constexpr SDL_Color SDL_GREEN_RGB = {54, 96, 81};

//RGB color information
static constexpr size_t WHITE_RGB[3] = {255, 255, 255};
static constexpr size_t YELLOW_RGB[3] = {148, 139, 77};
static constexpr size_t LT_GREEN_RGB[3] = {92, 133, 108};
static constexpr size_t OPAQUE = 255;

//Position of color in the fontColor array
static constexpr size_t WHITE = 0;
static constexpr size_t GREEN = 1;
static constexpr size_t BLACK = 2;
static constexpr size_t RED = 3;


class GameStateMenu : public GameState {
public:

    GameStateMenu(Game& g, int gameWidth, int gameHeight);

    virtual ~GameStateMenu();

    virtual bool load();
    virtual void loop();

    TTF_Font* headingFont;
    TTF_Font* textboxFont;
    TTF_Font* menuFont;
    LTexture frameFPSTextTexture;

private:

    Level level;
    Camera camera;

    virtual void sync() override;
    virtual void handle() override;
    virtual void update(const float delta) override;
    virtual void render() override;
    void positionElements();
    void renderText(LTexture *fontTexture, const char* text, SDL_Color color, TTF_Font* font, SDL_Rect rect);

    std::string menuItems[NUM_MENU_ITEMS]; //Menu Item text to be displayed in the window
    bool selected[NUM_MENU_ITEMS]; //Whether the menu option has been selected
    SDL_Rect menuItemPos[NUM_MENU_ITEMS]; //Position of the menu items on the screen
    LTexture menuTextTextures[NUM_MENU_ITEMS];

    bool activeTextbox[NUM_MENU_ITEMS]; //Whether the text box has been selected
    SDL_Rect textboxPos[NUM_TEXT_FIELDS]; //Position of the textbox on the screen
    LTexture textboxTextures[NUM_TEXT_FIELDS];

    SDL_Rect textboxTextPos[NUM_TEXT_FIELDS]; //Position of the textbox text on the screen
    std::string defaultText[NUM_TEXT_FIELDS]; //Default text to be displayed in the textbox
    std::string textInput[NUM_TEXT_FIELDS]; //User inputted text in the textbox

    SDL_Color   fontColors[4]; //Colors used in the Window

};

#endif
