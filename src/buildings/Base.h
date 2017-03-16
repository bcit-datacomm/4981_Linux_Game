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
static constexpr int BASE_WIDTH  = 500;
static constexpr int BASE_HEIGHT = 500;

// map width/height
static constexpr int MAP_WIDTH   = 2000;
static constexpr int MAP_HEIGHT  = 2000;

//The gab between the spawn point and base.
static constexpr int GAP = 100;

typedef std::pair<float, float> Point;

class Base : public Object {
public:
    Base(int health = 100);
    virtual ~Base();

    void onCollision();
    void collidingProjectile(int damage);
    Point getSpawnPoint();

private:
    int health;

};
#endif
