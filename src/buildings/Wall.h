#ifndef WALL_H
#define WALL_H
#include "../basic/Entity.h"
#include "Object.h"


typedef std::pair<float, float> Point;

class Wall : public Object {
public:
    Wall(const int32_t nid, const SDL_Rect& dest, const SDL_Rect& movementSize,
        const SDL_Rect& pickupSize, const int height, const int width);
    virtual ~Wall();

    void onCollision();
    void collidingProjectile(const int damage);

};

#endif
