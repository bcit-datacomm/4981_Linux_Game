#ifndef BASE_H
#define BASE_H
//#include <string>
//#include "LTexture.h"
//#include "HitBox.h"
#include "../basic/Entity.h"
#include "Object.h"
//#include "CollisionHandler.h"
//#include "Player.h"
//#include <vector>
//#include <map>
//#include <utility>
//#include <SDL2/SDL.h>

// base width/height
static constexpr int BASE_WIDTH = 1050;
static constexpr int BASE_HEIGHT = 1050;

// map width/height
static constexpr int MAP_WIDTH = 100 * 150;
static constexpr int MAP_HEIGHT = 100 * 150;

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
