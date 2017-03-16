#include "Base.h"
#include "../player/Marine.h"
#include<random>
#include "../log/log.h"

Base::Base(int health) : Object(BASE_HEIGHT, BASE_WIDTH), health(health) {
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

Point Base::getSpawnPoint() {

    //random number generator
    std::random_device rd;
    std::mt19937 eng(rd());

    //range 0 to 3 to be used for choosing North, South, West or East of Base
    std::uniform_int_distribution<> distr(0,3);

    int x;
    int y;

    switch(distr(eng)){
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
        x-=MARINE_WIDTH;
    if(y < getY())
        y-=MARINE_HEIGHT;

    return Point(x,y);
}
