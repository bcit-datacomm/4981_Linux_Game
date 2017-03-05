#include "Zombie.h"
#include <math.h>
#include <random>
#include <utility>
#include "Astar.h"
using namespace std;

#define PI 3.14159265
<<<<<<< HEAD
#define MAP_HEIGHT 2000
#define MAP_WIDTH 2000

Zombie::Zombie()  {
	zombieSpriteClips[0].x = 0;
	zombieSpriteClips[0].y = 0;
	zombieSpriteClips[0].w = 100;
	zombieSpriteClips[0].h = 100;
	this->movementHitBox.setRect(zombieSpriteClips[0]);
	this->movementHitBox.move(this->getX(), this->getY());
	this->movementHitBox.attached = this;
	this->projectileHitBox.setRect(zombieSpriteClips[0]);
	this->projectileHitBox.move(this->getX(), this->getY());
	this->projectileHitBox.attached = this;
	this->damageHitBox.setRect(zombieSpriteClips[0]);
	this->damageHitBox.move(this->getX(), this->getY());
	this->damageHitBox.attached = this;

	//this->setAngle(this->getRandomAngle());

	path = buildCoordinatePath(getX(), getY(), MAP_HEIGHT/2, MAP_WIDTH/2);

	printf("Create Zombie\n");
=======
#define ZOMBIE_VELOCITY 200

Zombie::Zombie(int health, int state) : Movable(ZOMBIE_VELOCITY), health(health), state(state) {
    setAngle(getRandomAngle());
    printf("Create Zombie\n");
>>>>>>> 6ad3220451911c42b263705814964620ec216bf0
}

Zombie::~Zombie() {
    printf("Destory Zombie\n");
}

void Zombie::onCollision() {
    // Do nothing for now
}

void Zombie::collidingProjectile(int damage) {
    health = health - damage;
}

<<<<<<< HEAD
void Zombie::generateMove(){
	bool goingUp;
	bool goingRight;

	pair<int, int> nextStep = path.front();
	path.erase(path.begin());

	int x = nextStep.first;
	int y = nextStep.second;

	this->setDX(getVelocity() + (x + getX()));
	this->setDY(getVelocity() + (y + getY()));
=======
int Zombie::getRandomAngle(){
    //random number generator
    std::random_device rd;
    std::mt19937 eng(rd());

    //range 0 to 360 degrees for zombie's angle
    std::uniform_int_distribution<> distr(0,360);

    return distr(eng);
}
>>>>>>> 6ad3220451911c42b263705814964620ec216bf0


}
/*
void Zombie::generateRandomMove(){
<<<<<<< HEAD
	int tx = this->getX()+50;
	int ty = this->getY()+50;
	if(tx<=0 || tx>=2000 || ty<=0 || ty>=2000){
		this->setAngle(this->getRandomAngle());
	}

	double cosVal = cos(this->getAngle()*PI/180.0);
	double sinVal = sin(this->getAngle()*PI/180.0);

	int x = this->getVelocity();
	int y = this->getVelocity();

	this->setDX(x*cosVal);
	this->setDY(y*sinVal);
=======
    int tx = getX()+50;
    int ty = getY()+50;
    if(tx<=0 || tx>=2000 || ty<=0 || ty>=2000){
        setAngle(getRandomAngle());
    }

    double cosVal = cos(getAngle()*PI/180.0);
    double sinVal = sin(getAngle()*PI/180.0);

    int x = getVelocity();
    int y = getVelocity();

    setDX(x*cosVal);
    setDY(y*sinVal);
>>>>>>> 6ad3220451911c42b263705814964620ec216bf0
}
*/
