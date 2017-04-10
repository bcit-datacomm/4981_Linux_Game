#include<random>

#include "Base.h"
#include "../player/Marine.h"
#include "../log/log.h"

Base::Base(const int32_t nid, const SDL_Rect& dest, const int health): Object(nid, dest, BASE_HEIGHT, BASE_WIDTH),
        health(health) {
    setX((MAP_WIDTH / 2) - BASE_WIDTH / 2);
    setY((MAP_HEIGHT / 2) - BASE_HEIGHT / 2);

    logv("Create Base\n");
}

Base::~Base() {
    logv("Destroy Base\n");
}

void Base::onCollision() {
    logv("Base collision\n");
}

void Base::collidingProjectile(int damage) {
    health -= damage;
}

/**
* Date: April 6, 2017
*
* Designer: Trista Huang
*
* Programmer: Trista Huang
*
* Function Interface: void Base::updateBaseImage()
*
* Description:
*       This function checks for base health everytime an update happens,
*       and changes base image accordingly.
*       It is called from GameManager::updateBase every frame.
*/
void Base::updateBaseImage() {
    const int healthNow = getHealth();
    int baseStartX;

    if (healthNow >= BASE_75_HP) {
        baseStartX = BASE_100;
    } else if (healthNow >= BASE_50_HP) {
        baseStartX = BASE_75;
    } else if (healthNow >= BASE_25_HP) {
        baseStartX = BASE_50;
    } else {
        // Expand rect to render this bigger image correctly
        Entity::setW(BASE_WIDTH_WIDE);
        Entity::setX(BASE_25_WIDE);
        Entity::updateRectHitBoxes();
        setSrcRect(BASE_25, 0, BASE_SRC_W_WIDE, BASE_SRC_H);
        return;
    }
    // Reset rect back to original size
    Entity::setX((MAP_WIDTH / 2) - BASE_WIDTH / 2);
    Entity::setW(BASE_WIDTH);
    Entity::updateRectHitBoxes();
    setSrcRect(baseStartX, BASE_SRC_Y, BASE_SRC_W, BASE_SRC_H);
}

Point Base::getSpawnPoint() {

    //random number generator
    std::random_device rd;
    std::mt19937 eng(rd());

    //range 0 to 3 to be used for choosing North, South, West or East of Base
    std::uniform_int_distribution<> distr(0,3);

    int x;
    int y;


    switch(distr(eng)) {

        case 0://North

            //y point is fixed
            y = getY() - GAP;
            //x point is randomly generated;
            x = distr(eng, decltype(distr)::param_type(getX() - GAP,
                    getX() + getWidth() + GAP));

            break;
        case 1://South
            y = getY() + getHeight() + GAP;
            x = distr(eng, decltype(distr)::param_type(getX() - GAP,
                    getX()+  getWidth() + GAP));
            break;

        case 2://West
            //x point is fixed
            x = getX() - GAP;
            //y point is randomly generated
            y = distr(eng, decltype(distr)::param_type(getY() - GAP,
                    getY() + getHeight() + GAP));
            break;

        case 3://East
            x = getY() + getWidth() + GAP;
            y = distr(eng, decltype(distr)::param_type(getY() - GAP,
                    getY() + getHeight() + GAP));
            break;
    }

    //considering the player size
    if(x < getX())
        x -= MARINE_WIDTH;
    if(y < getY())
        y -= MARINE_HEIGHT;

    return Point(x,y);
}
