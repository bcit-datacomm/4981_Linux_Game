/**
    Target.h

    DISCRIPTION:
        This is used to transfer targeting information from the colision handeler to
        other classes that will need to use it such as InstantWeapon.

        Objects of this type will primarily be sorted using the playerDist parameter. 
        Hence the operator< has been adjusted to use playerDist.

    AUTHOR: Deric Mccadden 3/20/2017

*/
#ifndef TARGET_H
#define TARGET_H

#include "../../basic/Entity.h"

#define TYPE_ZOMBIE 0
#define TYPE_WALL   1
#define TYPE_TURRET 2

class Target{
public:
    Target(Entity& t, int type, int hitX, int hitY, int playerDist) 
    : entity(t), type(type), hitX(hitX), hitY(hitY), playerDist(playerDist){}


    Target(const Target& t) : entity(t.entity), type(t.type), hitX(t.hitX), 
            hitY(t.hitY), playerDist(t.playerDist) {
    }
    
    Target& operator=(Target&& t) {
    	return t;
    }

    bool operator<(const Target& rhs) const {
        return playerDist < rhs.playerDist;
    }

    Entity& getEntity() const {return entity;}
    int getType() const {return type;}
    int getHitX() const {return hitX;}
    int getHitY() const {return hitY;}


private:
	Entity& entity;
    int type;
    int hitX;
    int hitY;
    int playerDist;
};


#endif