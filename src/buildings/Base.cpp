#include "Base.h"
#include "../player/Marine.h"
#include<random>
#include "../log/log.h"

Base::Base(int32_t nid, const SDL_Rect dest, int health): Object(nid, dest, BASE_HEIGHT, BASE_WIDTH),
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

Point Base::getSpawnPoint() {

    //random number generator
    std::random_device rd;
    std::mt19937 eng(rd());

    //range 0 to 3 to be used for choosing North, South, West or East of Base
    std::uniform_int_distribution<> distr(0,3);

    int nx;
    int ny;

    //The gab between the spawn point and base.
    int gab = 100;


    switch(distr(eng)) {

        case 0://North
            //y point is fixed
            ny = getY() - gab;
            //x point is randomly generated;
            nx = distr(eng, decltype(distr)::param_type(getX() - gab,
                       getX() + getWidth() + gab));
            break;
        case 1://South
            ny = getY() + getHeight() + gab;
            nx = distr(eng, decltype(distr)::param_type(getX() - gab,
                       getX()+  getWidth() + gab));
            break;

        case 2://West
            //x point is fixed
            nx = getX() - gab;
            //y point is randomly generated
            ny = distr(eng, decltype(distr)::param_type(getY() - gab,
                       getY() + getHeight() + gab));
            break;

        case 3://East
            nx = getY() + getWidth() + gab;
            ny = distr(eng, decltype(distr)::param_type(getY() - gab,
                       getY() + getHeight() + gab));
            break;
    }

    //considering the player size
    if(nx < getX())
        nx -= MARINE_WIDTH;
    if(ny < getY())
        ny -= MARINE_HEIGHT;

    return Point(nx,ny);
}
