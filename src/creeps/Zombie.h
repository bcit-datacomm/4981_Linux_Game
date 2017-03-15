#ifndef ZOMBIE_H
#define ZOMBIE_H
#include <string>
#include "../sprites/LTexture.h"
#include "../collision/HitBox.h"
#include "../basic/Entity.h"
#include "../collision/CollisionHandler.h"
#include "../inventory/Inventory.h"
#include "../buildings/Base.h"
#include <math.h>
#include <random>
#include <vector>
#include <utility>
#include <SDL2/SDL.h>
#include "../view/Window.h"
#include "../basic/Movable.h"

static constexpr int ZOMBIE_VELOCITY = 50;
static constexpr int ZOMBIE_INIT_HP  = 100;
static constexpr int ZOMBIE_FRAMES   = 50;
static constexpr int ZOMBIE_HEIGHT   = 125;
static constexpr int ZOMBIE_WIDTH    = 75;

// 8 possible directions
static constexpr int DIR_CAP = 8;
static constexpr int DIR_R   = 0;
static constexpr int DIR_RD  = 1;
static constexpr int DIR_D   = 2;
static constexpr int DIR_LD  = 3;
static constexpr int DIR_L   = 4;
static constexpr int DIR_LU  = 5;
static constexpr int DIR_U   = 6;
static constexpr int DIR_RU  = 7;

// Cardinal directions for setting angles
static constexpr int NORTH     = 0;
static constexpr int NORTHEAST = 45;
static constexpr int EAST      = 90;
static constexpr int SOUTHEAST = 135;
static constexpr int SOUTH     = 180;
static constexpr int SOUTHWEST = 225;
static constexpr int WEST      = 270;
static constexpr int NORTHWEST = 315;

// overlapped
static constexpr float OVERLAP = 0.1f;

// zombie state
typedef enum {
    ZOMBIE_IDLE,
    ZOMBIE_MOVE,
    ZOMBIE_ATTACK,
    ZOMBIE_DIE
} ZombieState;

class Zombie : public Movable {
public:
    Zombie(int health = ZOMBIE_INIT_HP, ZombieState state = ZOMBIE_IDLE, int step = 0,
           int dir = -1, int frame = ZOMBIE_FRAMES);
    virtual ~Zombie();

    void onCollision();

    void collidingProjectile(int damage);

    void generateMove();            // A* movement

    bool isMoving();                // Returns if the zombie should be moving

    bool checkTarget();               // checks if the zombie already arrived at the target

    int getMoveDir();               // get move direction

    bool checkBounds(const float x, const float y) const;  // boundary checks

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
    void setState(const ZombieState state_) {
        state = state_;
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
    void setCurDir(const int d) {
        dir = d;
    }

    /**
     * Get current direction
     * Fred Yang
     * March 14
     */
    int getCurDir() const {
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
    int dir;            // moving direction
    int frame;          // frames per tile
};

#endif
