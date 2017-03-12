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

const int ZOMBIE_HEIGHT = 125;
const int ZOMBIE_WIDTH = 75;



class Zombie : public Movable {
public:
    Zombie(int health = 100, int state = 0);
    virtual ~Zombie();

	void onCollision();

	void collidingProjectile(int damage);

    void generateMove();            // A* movement

    void setStep(int sp);           // set step

    int getStep();                  // get step

    float getEndY();                // get the y-coordinate for the end of the step

    float getEndX();                // get the x-coordinate for the end of the step

    bool isMoving();                // Returns if the zombie should be moving

    void checkMove();               // Does a check to see if zombie should move

    void setEnd(float X, float Y);  // Sets the end coordinates of a zombie step

    std::string getPath();          // get path

    void setPath(std::string pth);  // set path

    int getDir();                   // get move direction

    bool checkBound(float x, float y);  // boundary checks
    // A* path
    std::string generatePath(const int& xStart, const int& yStart,
                             const int& xDest, const int& yDest);

private:
    int health;         // health of zombie
    int state;          // used to select sprite to display
    std::string path;   // A* path zombie should follow
    int step;           // Number of steps zombie has taken in path
    bool moving;        // Whether or not zombie is moving
    float endX;         // X coordinate of the end of a step
    float endY;         // Y coordinate of the end of a step
};

#endif
