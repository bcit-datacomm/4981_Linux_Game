#ifndef MARINE_H
#define MARINE_H

#include <string>
#include <math.h>
#include <vector>
#include <SDL2/SDL.h>

#include "../basic/Entity.h"
#include "../basic/Movable.h"
#include "../collision/CollisionHandler.h"
#include "../inventory/Inventory.h"
#include "../view/Window.h"

//speed of marine in relation to current frame duration / desired frame duration
static constexpr int MARINE_VELOCITY = 400;
//width of any marine image
static constexpr int SPRITE_SIZE_X = 75;
//height of any marine image
static constexpr int SPRITE_SIZE_Y = 125;
//speed of walking animation ie frames till next animation
static constexpr int FRAME_COUNT_WALK = 7;

static constexpr int SPRITE_RIGHT = SPRITE_SIZE_Y * 2;
static constexpr int SPRITE_BACK_RIGHT = SPRITE_SIZE_Y * 3;
static constexpr int SPRITE_BACK = SPRITE_SIZE_Y * 4;
static constexpr int SPRITE_BACK_LEFT = SPRITE_SIZE_Y * 5;
static constexpr int SPRITE_LEFT = SPRITE_SIZE_Y * 6;
static constexpr int SPRITE_FRONT_LEFT = SPRITE_SIZE_Y * 7;
static constexpr int SPRITE_FRONT = 0;
static constexpr int SPRITE_FRONT_RIGHT = SPRITE_SIZE_Y;
static constexpr int SPRITE_NEXT_STEP = SPRITE_SIZE_Y;

static constexpr double SPRITE_ANGLE1 = M_PI / 8;
static constexpr double SPRITE_ANGLE2 = -(M_PI / 8);
static constexpr double SPRITE_ANGLE3 = -3 * M_PI / 8;
static constexpr double SPRITE_ANGLE4 = -5 * M_PI / 8;
static constexpr double SPRITE_ANGLE5 = -7 * M_PI / 8;
static constexpr double SPRITE_ANGLE6 = -9 * M_PI / 8;
static constexpr double SPRITE_ANGLE7 = -11 * M_PI / 8;
static constexpr double SPRITE_ANGLE8 = 3 * M_PI / 8;

const int MARINE_HEIGHT = 125;
const int MARINE_WIDTH = 75;

class Marine : public Movable {
public:
    Marine(const int32_t id, const SDL_Rect& dest, const SDL_Rect& movementSize,
        const SDL_Rect& projectileSize, const SDL_Rect& damageSize);
    virtual ~Marine();

    void create(); //function displays character
    void onCollision();
    int getHealth() const {return health;}
    void collidingProjectile(const int damage);
    bool fireWeapon();
    int32_t checkForPickUp();
    int getHealth() {return health;};
    void updateImageDirection();
    void updateImageWalk();
    void activateStore(const Entity *ep);
    Inventory inventory;
    void setHealth(const int hlth) {health = hlth;};
    int getCurrentHealth() {return health;}; //returns the current health of the marine

private:
    std::string username;
    int health = 100;
    int state; //used to select sprite to display
    static int frameCount;
};

#endif
