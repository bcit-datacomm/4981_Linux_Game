#ifndef ZOMBIE_H
#define ZOMBIE_H
#include <string>
#include "LTexture.h"
#include "HitBox.h"
#include "Entity.h"
#include "CollisionHandler.h"
#include "Inventory.h"
#include <math.h>
#include <random>
#include <vector>
#include <utility>
#include <SDL2/SDL.h>
#include "Window.h"
#include "Movable.h"

const int ZOMBIE_VELOCITY = 50;
const int ZOMBIE_INIT_HP = 100;
const int ZOMBIE_FRAMES = 50;
const int ZOMBIE_HEIGHT = 125;
const int ZOMBIE_WIDTH = 75;

class Zombie : public Movable {
public:
    Zombie(int health = ZOMBIE_INIT_HP, ZOMBIE_STATE state = ZOMBIE_IDLE, int step = 0,
           int dir = -1, int frame = ZOMBIE_FRAMES);
    virtual ~Zombie();

    void onCollision();

    void collidingProjectile(int damage);

    void generateMove();            // A* movement

    void setStep(const int sp);     // set step

    int getStep() const;            // get step
    
    void setState(const ZOMBIE_STATE stat); // set state
    
    ZOMBIE_STATE getState() const;  // get state

    bool isMoving();                // Returns if the zombie should be moving

    bool checkBase();               // checks if the zombie already arrived at the base

    std::string getPath() const;    // get path

    void setPath(const std::string pth); // set path

    int getMoveDir();               // get move direction
    
    void setCurDir(const int d);    // set current direction
    
    int getCurDir() const;          // get current direction
    
    void setCurFrame(const int frm); // set current frame
    
    int getCurFrame() const;        // get current frame
    
    bool checkBounds(const float x, const float y) const;  // boundary checks
    
    // A* path
    std::string generatePath(const float xStart, const float yStart,
                             const float xDest, const float yDest);
                             
    // overlapped method
    bool overlapped(const float x1, const float y1, const int w1, const int h1, 
                    const float x2, const float y2, const int w2, const int h2, 
                    const float overlap);

private:
    int health;         // health points of zombie
    std::string path;   // A* path zombie should follow
    ZOMBIE_STATE state; // 0 - idle, 1 - move, 2 - attack, 3 - die
    int step;           // Number of steps zombie has taken in path
    int dir;            // moving direction
    int frame;          // frames per tile
};

#endif
