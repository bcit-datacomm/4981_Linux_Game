#ifndef MARINE_H
#define MARINE_H
#include <string>
#include "../sprites/LTexture.h"
#include "../basic/Entity.h"
#include "../basic/Movable.h"
#include "../collision/CollisionHandler.h"
#include "../inventory/Inventory.h"
#include <math.h>
#include <vector>
#include <SDL2/SDL.h>
#include "../UDPHeaders.h"
#include "../view/Window.h"


#define MARINE_VELOCITY 500

const int MARINE_HEIGHT = 100;
const int MARINE_WIDTH = 100;

class Marine : public Movable {
public:

    void create(); //function displays character
    void onCollision();
    void collidingProjectile(int damage);
    void fireWeapon();
    void checkForPickUp();

    Marine(std::string uname);
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
    int pickupTick = 0;
    int pickupDelay = 200;

};

#endif
