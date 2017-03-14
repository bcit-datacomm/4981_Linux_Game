#include "Base.h"
#include "../player/Marine.h"
#include<random>

Base::Base(int health) : Object(BASE_HEIGHT, BASE_WIDTH), health(health) {
    setX((MAP_WIDTH / 2) - BASE_WIDTH / 2);
    setY((MAP_HEIGHT / 2) - BASE_HEIGHT / 2);

    printf("Create Base\n");
}

Base::~Base() {
    printf("Destory Base\n");
}
void Base::onCollision() {
    printf("Base collision\n");
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

    //The gab between the spawn point and base.
    int gab = 100;


    switch(distr(eng)){
        case 0://North

            //y point is fixed
            y = getY() - gab;
            //x point is randomly generated;
            x = distr(eng, decltype(distr)::param_type(getX() - gab,
                                                       getX() + getWidth() + gab));

            break;
        case 1://South
            y = getY() + getHeight() + gab;
            x = distr(eng, decltype(distr)::param_type(getX() - gab,
                                                       getX()+  getWidth() + gab));
            break;
        case 2://West

            //x point is fixed
            x = getX() - gab;
            //y point is randomly generated
            y = distr(eng, decltype(distr)::param_type(getY() - gab,
                                                       getY() + getHeight() + gab));
            break;
        case 3://East
            x = getY() + getWidth() + gab;
            y = distr(eng, decltype(distr)::param_type(getY() - gab,
                                                       getY() + getHeight() + gab));
            break;
    }

    //considering the player size
    if(x < getX())
        x-=MARINE_WIDTH;
    if(y < getY())
        y-=MARINE_HEIGHT;

    return Point(x,y);
}
