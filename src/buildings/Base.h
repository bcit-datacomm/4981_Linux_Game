#ifndef BASE_H
#define BASE_H
#include "../basic/Entity.h"
#include "Object.h"

// base width/height
static constexpr int BASE_WIDTH = 750;
static constexpr int BASE_WIDTH_WIDE = 940;
static constexpr int BASE_HEIGHT = 750;

// map width/height
static constexpr int MAP_WIDTH = 50 * 250;
static constexpr int MAP_HEIGHT = 50 * 250;

// Starting point and size of base image to render
static constexpr int BASE_SRC_X = 82;
static constexpr int BASE_SRC_Y = 54;
static constexpr int BASE_SRC_W = 990;
static constexpr int BASE_SRC_H = 1070;
static constexpr int BASE_SRC_W_WIDE = 1240;

// Base x-coordinate for each states
static constexpr int BASE_100 = BASE_SRC_X;
static constexpr int BASE_75 = 1214;
static constexpr int BASE_50 = 2416;
static constexpr int BASE_25 = 3556;
static constexpr int BASE_25_WIDE = (MAP_WIDTH / 2) - BASE_WIDTH / 2 - 85;

// Base HP indicator, used to update base image
static constexpr int BASE_INIT_HP = 100;
static constexpr int BASE_75_HP = BASE_INIT_HP * 0.75;
static constexpr int BASE_50_HP = BASE_INIT_HP * 0.50;
static constexpr int BASE_25_HP = BASE_INIT_HP * 0.25;

//The gab between the spawn point and base.
static constexpr int GAP = 150;

typedef std::pair<float, float> Point;

class Base : public Object {
public:
    Base(const int32_t nid = 0, const SDL_Rect& dest = {
        MAP_WIDTH / 2, MAP_HEIGHT / 2, BASE_WIDTH, BASE_HEIGHT}, int health = BASE_INIT_HP);
    virtual ~Base();

    void onCollision();
    void collidingProjectile(int damage);
    void setHealth(const int newHealth) {health = newHealth;}
    int getHealth() {return health;}
    int getLastHealth() const {return lastHealth;}
    void updateBaseImage();
    Point getSpawnPoint();

private:
    int health;
    int lastHealth;
};
#endif
