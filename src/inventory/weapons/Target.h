#ifndef TARGET_H
#define TARGET_H

#include "../../basic/Entity.h"


struct Target{
    Entity target;
    int hitX;
    int hitY;
    int playerDist;
    Target(Entity& t) : target(t){}

    Target(const Target& t) : target(t.target), hitX(t.hitX), 
            hitY(t.hitY), playerDist(t.playerDist) {
    }
    Target& operator=(Target&& t){return t;}

    bool operator<(const Target& rhs) const {
        return playerDist < rhs.playerDist;
    }
};


#endif