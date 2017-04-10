/*------------------------------------------------------------------------------
* Header: Zombie.h
*
* Functions:
*
*
* Date:
*
* Revisions:
* Edited By : Yiaoping Shu- Style guide
*
* Designer:
*
* Author:
*
* Notes:
*
------------------------------------------------------------------------------*/
#ifndef ZOMBIE_H
#define ZOMBIE_H

#include <string>
#include <cmath>
#include <random>
#include <vector>
#include <utility>
#include <SDL2/SDL.h>

#include "../collision/HitBox.h"
#include "../basic/Entity.h"
#include "../collision/CollisionHandler.h"
#include "../inventory/Inventory.h"
#include "../collision/Quadtree.h"
#include "../buildings/Base.h"
#include "../view/Window.h"
#include "../basic/Movable.h"

static constexpr int ZOMBIE_VELOCITY = 400;
static constexpr int ZOMBIE_SIGHT = 500;
static constexpr int ZOMBIE_INIT_HP = 100;
static constexpr int ZOMBIE_FRAMES = 50;
static constexpr int ZOMBIE_HEIGHT = 125; // y value
static constexpr int ZOMBIE_WIDTH = 75; // x value
static constexpr int ZOMBIE_DEAD_WIDTH = 122; // dead zombie x value
static constexpr int FRAME_COUNT_ZOMBIE = 10; // speed of walking animation
static constexpr int CHECK_RATE = 15; // 60 / 4 times 
static constexpr int ANGLE_UPDATE_RATE = 5;
static constexpr int ATTACK_DURATION = 10;
static constexpr int HIT_DURATION = 15;

static constexpr int ZOMBIE_RIGHT = ZOMBIE_HEIGHT * 2;
static constexpr int ZOMBIE_BACK_RIGHT = ZOMBIE_HEIGHT * 3;
static constexpr int ZOMBIE_BACK = ZOMBIE_HEIGHT * 4;
static constexpr int ZOMBIE_BACK_LEFT = ZOMBIE_HEIGHT * 5;
static constexpr int ZOMBIE_LEFT = ZOMBIE_HEIGHT * 6;
static constexpr int ZOMBIE_FRONT_LEFT = ZOMBIE_HEIGHT * 7;
static constexpr int ZOMBIE_FRONT = 0;
static constexpr int ZOMBIE_FRONT_RIGHT = ZOMBIE_HEIGHT;
static constexpr int ZOMBIE_DEAD = ZOMBIE_HEIGHT * 8;
static constexpr int ZOMBIE_HIT_X = ZOMBIE_WIDTH * 3;
static constexpr int ZOMBIE_ATTACK_X = ZOMBIE_WIDTH * 4;
//Isaac Morneau April 6, 2017
//
//these are not a duplicate of marine, these need to be seperate because
//marine has its angle caluclated in screen coords with the mouse while
//this one has its angle calculated from atan2 normalized to be from 0-2pi
static constexpr double ZOMBIE_SPRITE_ANGLE1 = 0;
static constexpr double ZOMBIE_SPRITE_ANGLE2 = M_PI / 8;
static constexpr double ZOMBIE_SPRITE_ANGLE3 = 3 * M_PI / 8;
static constexpr double ZOMBIE_SPRITE_ANGLE4 = 5 * M_PI / 8;
static constexpr double ZOMBIE_SPRITE_ANGLE5 = 7 * M_PI / 8;
static constexpr double ZOMBIE_SPRITE_ANGLE6 = 9 * M_PI / 8;
static constexpr double ZOMBIE_SPRITE_ANGLE7 = 11 * M_PI / 8;
static constexpr double ZOMBIE_SPRITE_ANGLE8 = 13 * M_PI / 8;
static constexpr double ZOMBIE_SPRITE_ANGLE9 = 2 * M_PI;

class Zombie : public Movable {
public:
    Zombie(const int32_t id, const SDL_Rect& dest, const SDL_Rect& movementSize, const SDL_Rect& projectileSize,
        const SDL_Rect& damageSize, const int health = ZOMBIE_INIT_HP);

    virtual ~Zombie();

    void move(const float moveX, const float moveY, CollisionHandler& ch);

    void collidingProjectile(int damage);

    int getHealth() const {return health;}
    void setHealth(const int h) {health = h;}

    void update();
    void updateImageWalk();
    void updateImageDirection();

private:
    int health;// health points of zombie
    int frameCount;//counts frames for animation
    int ignore;//how long to not track for
    int flipper;//do we need to go the other way?
    int actionTick;//when the action started
    char action;
    Inventory inventory;//inventory holds a weapon used to attack
    void zAttack();//ATTACK!
};

#endif
