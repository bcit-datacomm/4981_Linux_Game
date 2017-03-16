#ifndef ZOMBIE_H
#define ZOMBIE_H
#include <string>
#include <math.h>
#include <random>
#include <vector>
#include <utility>
#include <SDL2/SDL.h>
#include "../sprites/LTexture.h"
#include "../collision/HitBox.h"
#include "../basic/Entity.h"
#include "../collision/CollisionHandler.h"
#include "../inventory/Inventory.h"
#include "../buildings/Base.h"
#include "../view/Window.h"
#include "../basic/Movable.h"

static constexpr int ZOMBIE_VELOCITY = 50;
static constexpr int ZOMBIE_INIT_HP  = 100;
static constexpr int ZOMBIE_FRAMES   = 50;
static constexpr int ZOMBIE_HEIGHT   = 125;
static constexpr int ZOMBIE_WIDTH    = 75;

// 8 possible directions
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

// Cardinal directions for setting angles
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

// overlapped
static constexpr float OVERLAP = 0.1;

// zombie state
enum class ZombieState {
    ZOMBIE_IDLE,
    ZOMBIE_MOVE,
    ZOMBIE_ATTACK,
    ZOMBIE_DIE
};

class Zombie : public Movable {
public:
    Zombie(int health = ZOMBIE_INIT_HP, ZombieState state = ZombieState::ZOMBIE_IDLE, int step = 0,
           ZombieDirection dir = ZombieDirection::DIR_INVALID, int frame = ZOMBIE_FRAMES);
    virtual ~Zombie();

    void onCollision();

    void collidingProjectile(int damage);

    void generateMove();            // A* movement

    bool isMoving() const;                // Returns if the zombie should be moving

    bool checkTarget() const;               // checks if the zombie already arrived at the target

    ZombieDirection getMoveDir() const;               // get move direction

    static constexpr bool checkBounds(const float x, const float y);  // boundary checks

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
};

#endif
