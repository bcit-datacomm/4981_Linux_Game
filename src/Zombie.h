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
#include <SDL2/SDL.h>
#include "Window.h"

#define PI 3.14159265
const int ZOMBIE_HEIGHT = 100;
const int ZOMBIE_WIDTH = 100;

class Zombie : public Entity {
public:

	void move(float moveX, float moveY, CollisionHandler* ch); // Moves Zombie

	void setDX(float px); //set delta x coordinate

	void setDY(float py); //set delta y coordinate

	void setVelocity(int pvel); // set velocity of Zombie movement

	float getDX(); // get delta x coordinate

	float getDY(); //get delta y coordinate

	int getVelocity(); // get velocity of Zombie movement

	void onCollision();

	void collidingProjectile(int damage);

	void setAngle(double a);//sets angle of sprite to

	double getAngle(); //returns sprites angle

	void generateRandomMove();      // randomly generate the zombie's movement
    void generateMove();            // A* movement
	int getRandomAngle();           // randomly generate the angle of zombie
    void setStep(int sp);           // set step
    int getStep();                  // get step
    std::string getPath();          // get path
    void setPath(std::string pth);  // set path
    int getDir();                   // get move direction
    bool checkBound(float x, float y);  // boundary checks
    // A* path
    std::string generatePath(const int& xStart, const int& yStart,
                             const int& xDest, const int& yDest);

	Zombie();
	virtual ~Zombie();

	SDL_Rect zombieSpriteClips[1];
	HitBox movementHitBox;	// Hit box for movement
	HitBox projectileHitBox; // Hit box for projectiles
	HitBox damageHitBox; // Hit box for damage

private:
    float dx = 0; // delta x coordinate
    float dy = 0; // delta y coordinate
    double angle = 180.0;
    int velocity = 200; // velocity of Zombie movement
    int health = 100;
    int state; //used to select sprite to display
    int type = 0;  // 0 - minion, targeting turret; 1 - boss, targetting players
    int step = 0;
    std::string path;

};

#endif
