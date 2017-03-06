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

    std::string getPath();          // get path

    void setPath(std::string pth);  // set path

    int getDir();                   // get move direction

    bool checkBound(float x, float y);  // boundary checks
    // A* path
    std::string generatePath(const int& xStart, const int& yStart,
                             const int& xDest, const int& yDest);

private:
    int health;
    int state; //used to select sprite to display
    std::string path;
    int step = 0;
};

#endif
