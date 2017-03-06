#ifndef WALL_H
#define WALL_H
#include "Entity.h"
#include "Object.h"

//TODO :  change later.
constexpr int WALL_HEIGHT = 100;
constexpr int WALL_WIDTH = 100;

typedef std::pair<float, float> Point;

class Wall : public Object {
public:
    Wall();
    virtual ~Wall();

    void onCollision();
    void collidingProjectile(int damage);

private:

};
#endif
