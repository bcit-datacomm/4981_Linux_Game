#ifndef MARINE_H
#define MARINE_H
#include <string>
#include "LTexture.h"
#include "Entity.h"
#include "Movable.h"
#include "CollisionHandler.h"
#include "Inventory.h"
#include <math.h>
#include <vector>
#include <SDL2/SDL.h>
#include "Window.h"
#include "UDPHeaders.h"

#define MARINE_VELOCITY 500

const int MARINE_HEIGHT = 100;
const int MARINE_WIDTH = 100;

class Marine : public Movable {
public:
    void create(); //function displays character

    void onCollision();

    void collidingProjectile(int damage);

    void fireWeapon();

    Marine();
    virtual ~Marine();

    Inventory inventory;

    void setHealth(const int& hlth) {health = hlth;};
    void setUsername(const std::string& uname) {username = uname;};
    std::string& getUsername() {return username;};
private:
    std::string username;
    int health = 100;
    int state; //used to select sprite to display

};

#endif
