#ifndef WALL_H
#define WALL_H
#include "Entity.h"
#include "Object.h"


typedef std::pair<float, float> Point;

class Wall : public Object {
public:
    Wall(int32_t nid, const SDL_Rect dest, const SDL_Rect &spriteSize, const SDL_Rect &movementSize,
        const SDL_Rect &pickupSize, int height, int width);
    virtual ~Wall();

    void onCollision();
    void collidingProjectile(int damage);

private:

};
#endif
