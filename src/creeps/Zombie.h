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

static constexpr int ZOMBIE_VELOCITY = 50;
static constexpr int ZOMBIE_INIT_HP = 100;
static constexpr int ZOMBIE_FRAMES = 50;
static constexpr int ZOMBIE_HEIGHT = 125; // y value
static constexpr int ZOMBIE_WIDTH = 75; // x value
static constexpr int ZOMBIE_DEAD_WIDTH = 122; // dead zombie x value
static constexpr int FRAME_COUNT_ZOMBIE = 10; // speed of walking animation

static constexpr int ZOMBIE_RIGHT = ZOMBIE_HEIGHT * 2;
static constexpr int ZOMBIE_BACK_RIGHT = ZOMBIE_HEIGHT * 3;
static constexpr int ZOMBIE_BACK = ZOMBIE_HEIGHT * 4;
static constexpr int ZOMBIE_BACK_LEFT = ZOMBIE_HEIGHT * 5;
static constexpr int ZOMBIE_LEFT = ZOMBIE_HEIGHT * 6;
static constexpr int ZOMBIE_FRONT_LEFT = ZOMBIE_HEIGHT * 7;
static constexpr int ZOMBIE_FRONT = 0;
static constexpr int ZOMBIE_FRONT_RIGHT = ZOMBIE_HEIGHT;
static constexpr int ZOMBIE_NEXT_STEP = ZOMBIE_HEIGHT;
static constexpr int ZOMBIE_DEAD = ZOMBIE_HEIGHT * 8;


/* 8 possible directions combining left, right, up, down.
 * Fred Yang
 * Feb 14
 */
static constexpr int DIR_CAP = 8;
enum class ZombieDirection : int {
    DIR_R,
    DIR_RD,
    DIR_D,
    DIR_LD,
    DIR_L,
    DIR_LU,
    DIR_U,
    DIR_RU,
    DIR_INVALID = -1
};

/* Cardinal directions for setting angles, one angle for each movement direction.
 * Robert Arendac
 * March 14
 */
enum class ZombieAngles : int {
    NORTH = 0,
    NORTHEAST = 45,
    EAST = 90,
    SOUTHEAST = 135,
    SOUTH = 180,
    SOUTHWEST = 225,
    WEST = 270,
    NORTHWEST = 315
};

/* zombie states, change when you want zombie to take a different action.
 * Eg. go from moving to attacking
 * Fred Yang
 * March 14
 */
enum class ZombieState {
    ZOMBIE_IDLE,
    ZOMBIE_MOVE,
    ZOMBIE_ATTACK,
    ZOMBIE_DIE
};

class Zombie : public Movable {
public:
    Zombie(const int32_t id, const SDL_Rect& dest, const SDL_Rect& movementSize, const SDL_Rect& projectileSize,
        const SDL_Rect& damageSize, const int health = ZOMBIE_INIT_HP,
        const ZombieState state = ZombieState::ZOMBIE_IDLE,
        const int step = 0, const ZombieDirection dir = ZombieDirection::DIR_INVALID,
        const int frame = ZOMBIE_FRAMES);

    virtual ~Zombie();


    void onCollision();

    void collidingProjectile(int damage);

    void updateZombieWalk(int directionVal);

    int getHealth() const {return health;}
    void setHealth(const int h) {health = h;}

    void generateMove();                    // A* movement

    bool isMoving() const;                  // Returns if the zombie should be moving

    bool checkTarget() const;               // checks if the zombie already arrived at the target

    ZombieDirection getMoveDir() const;     // get move direction

    // A* path
    std::string generatePath(const float xStart, const float yStart,
            const float xDest, const float yDest);

    /**
     * Set steps taken
     * Fred Yang
     * Feb 14
     */
    void setStep(const int sp) {
        step = sp;
    }

    /**
     * Get steps taken
     * Fred Yang
     * Feb 14
     */
    int getStep() const {
        return step;
    }

    /**
     * Set state
     * Fred Yang
     * March 14
     */
    void setState(const ZombieState newState) {
        state = newState;
    }

    /**
     * Get state
     * Fred Yang
     * March 14
     */
    ZombieState getState() const {
        return state;
    }

    /**
     * Get A* path
     * Fred Yang
     * Feb 14
     */
    string getPath() const {
        return path;
    }

    /**
     * Set A* path
     * Fred Yang
     * Feb 14
     */
    void setPath(const string pth) {
        path = pth;
    }

    /**
     * Set direction
     * Fred Yang
     * March 14
     */
    void setCurDir(const ZombieDirection d) {
        dir = d;
    }

    /**
     * Get current direction
     * Fred Yang
     * March 14
     */
    ZombieDirection getCurDir() const {
        return dir;
    }

    /**
     * Set frame
     * Fred Yang
     * March 14
     */
    void setCurFrame(const int frm) {
        frame = frm;
    }

    /**
     * Get current frame
     * Fred Yang
     * March 14
     */
    int getCurFrame() const {
        return frame;
    }

private:
    int health;         // health points of zombie
    std::string path;   // A* path zombie should follow
    ZombieState state; // 0 - idle, 1 - move, 2 - attack, 3 - die
    int step;           // Number of steps zombie has taken in path
    ZombieDirection dir;            // moving direction
    int frame;          // frames per tile
    int frameCountZombie;
    Inventory inventory;//inventory holds a weapon used to attack

    void zAttack();
};

#endif
