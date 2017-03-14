#ifndef WALL_H
#define WALL_H
#include "../basic/Entity.h"
#include "Object.h"


typedef std::pair<float, float> Point;

class Wall : public Object {
public:
    Wall(int height, int width);
    virtual ~Wall();

    void onCollision();
    void collidingProjectile(int damage);

private:

};
#endif
