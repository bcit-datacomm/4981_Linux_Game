#ifndef PLAYER_H
#define PLAYER_H

#include <SDL2/SDL.h>
#include <string>
#include <memory>

#include "../player/Marine.h"
#include "../buildings/Base.h"
#include "../turrets/Turret.h"
#include "../inventory/Inventory.h"
#include "../client/NetworkManager.h"
#include "../UDPHeaders.h"

static constexpr double DOUBLE_COMPARISON_PRECISION = 0.001;
static constexpr int PLAYER_PLACE_DISTANCE = 100;
static constexpr int RESPAWN_DELAY = 3000;
static constexpr double PI = 3.14159265;

//all for the GUIDE CALCULATIONS
static constexpr int ONE_EIGHTY = 180;
static constexpr int NINTY_DEGREES = 90;
static constexpr int TWO_SEVENTY_DEGREES = 270;
static constexpr int ONE_SIXTY_DEGREES = 160;
static constexpr int MAX_WIDTH = 70;
static constexpr int HORIZONTAL_ADJUST = 175;
static constexpr int VERTICAL_ADJUST = 150;
static constexpr int BOTTOM_PADDING = 250;
static constexpr int TOP_AND_LEFT_PADDING = 50;
static constexpr int RIGHT_PADDING = 75;
static constexpr int TOP_LEFT_ANGLE = 145;
static constexpr int TOP_RIGHT_ANGLE = 40;
static constexpr int BOTTOM_RIGHT_ANGLE = 323;
static constexpr int BOTTOM_LEFT_ANGLE = 216;
static constexpr int GUIDE_SIZE = 100;

class Player {
public:
    Player();
    ~Player() = default;

    // Handles player input with keyboard state
    void handleKeyboardInput(const int winWidth, const int winHeight, const Uint8 *state);
    void handleMouseUpdate(const int winWidth, const int winHeight, const float camX, const float camY);

    void setControl(Marine* newControl);

    void handleMouseWheelInput(const SDL_Event *e);

    // Added by Mark.C 02/07/2017
    void handlePlacementClick(SDL_Renderer *renderer);

    void handleTempBarricade(SDL_Renderer *renderer);
    void handleTempTurret(SDL_Renderer *renderer);

    bool checkMarineState();
    void respawn(const Point& newPoint);


    void sendServMoveAction();
    void sendServAttackAction();
    bool hasChangedAngle() const;
    bool hasChangedCourse() const;
    void setId(const int32_t newId) {id = newId;};
    int32_t getId() const {return id;};

    //Stays as pointer cause the player gets a marine object after the ctor is called
    Marine * getMarine() const {return marine;}

    void spawnMapGuides(const int winWidth, const int winHeight);
    double  getAngleBetweenPoints(const std::pair<float, float> p1, const std::pair<float, float> p2);
    std::pair<float, float> getGuideCoord(const double angle, const int winWidth, const int winHeight);
    std::pair<float, float> calculateHorizontalCoords(const double angle, const int compareDegree,
            const int winWidth, const int winHeight);
    std::pair<float, float> calculateVerticleCoords(const double angle, const int compareDegree,
            const int winWidth, const int winHeight);
private:
    int tempBarricadeID;
    int tempTurretID;
    bool holdingTurret;
    int pickupTick;
    int pickupDelay;
    int shootDelay;
    int respawnTick;
    int32_t id;
    ClientMessage moveAction;
    ClientMessage attackAction;
    Marine *marine;
};

#endif
