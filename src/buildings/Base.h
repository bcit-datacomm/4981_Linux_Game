#ifndef BASE_H
#define BASE_H
#include "../basic/Entity.h"
#include "Object.h"

// base width/height
static constexpr int BASE_WIDTH = 750;
static constexpr int BASE_HEIGHT = 750;

// map width/height
static constexpr int MAP_WIDTH = 50 * 250;
static constexpr int MAP_HEIGHT = 50 * 250;

//The gab between the spawn point and base.
static constexpr int GAP = 150;

typedef std::pair<float, float> Point;

class Base : public Object {
public:
    Base(const int32_t nid = 0, const SDL_Rect& dest = {MAP_WIDTH / 2, MAP_HEIGHT / 2, BASE_WIDTH, BASE_HEIGHT}, int health = 100);
    virtual ~Base();

    void onCollision();
    void collidingProjectile(int damage);
    Point getSpawnPoint();

private:
    int health;

};
#endif
