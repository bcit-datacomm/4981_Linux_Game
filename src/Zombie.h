#ifndef ZOMBIE_H
#define ZOMBIE_H
#include <string>
#include "LTexture.h"
#include "HitBox.h"
#include "Entity.h"
#include "CollisionHandler.h"
#include "Inventory.h"
#include <math.h>
#include <vector>
#include <utility>
#include <SDL2/SDL.h>
#include "Window.h"
#include "Movable.h"

const int ZOMBIE_HEIGHT = 125;
const int ZOMBIE_WIDTH = 75;

<<<<<<< HEAD

class Zombie : public Entity {
=======
class Zombie : public Movable {
>>>>>>> 6ad3220451911c42b263705814964620ec216bf0
public:
    Zombie(int health = 100, int state = 0);
    virtual ~Zombie();

<<<<<<< HEAD
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

	void generateMove();


	Zombie();
	virtual ~Zombie();

	SDL_Rect zombieSpriteClips[1];
	HitBox movementHitBox;	// Hit box for movement
	HitBox projectileHitBox; // Hit box for projectiles
	HitBox damageHitBox; // Hit box for damage

=======
    void onCollision();
    void collidingProjectile(int damage);
    void generateRandomMove(); //randomly generate the zombie's movement
    int getRandomAngle(); //randomly generate the angle of zombie
>>>>>>> 6ad3220451911c42b263705814964620ec216bf0
private:
    int health;
    int state; //used to select sprite to display
	std::vector<std::pair<int, int>> path;

};

#endif
