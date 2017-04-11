/*------------------------------------------------------------------------------
* Source: Target.h       
*
* Functions:
*
* Date: 
*
* Revisions: 
* Edited By : Tim Makimov on 2017/APR/10
*
* Designer: 
*
* Author: 
*
* Notes:
------------------------------------------------------------------------------*/

/**
    Target.h

    DISCRIPTION:
        This is used to transfer targeting information from the colision handeler to
        other classes that will need to use it such as InstantWeapon.

    AUTHOR: Deric Mccadden 3/20/2017

*/
#ifndef TARGET_H
#define TARGET_H

#include "../../basic/Entity.h"

#define TYPE_ZOMBIE 0
#define TYPE_WALL   1
#define TYPE_TURRET 2



/**
    class Target

    DISCRIPTION:
        This is used to store targeting information for a single target.

    PARAMATERS:
        id of the Zombie which we will use to get the zombie itself when we want to damage it.

        hitX and hitY are where the zombie was hit by the bullet.

        playerDist represents the distance to the gun muzzle from (hitx, hity).
        
        Objects of this type will primarily be sorted using the playerDist parameter. 
        Hence the operator< has been adjusted to use playerDist.

    AUTHOR: Deric Mccadden 3/29/2017

*/
class Target{
public:
    Target(int32_t id, int type, int hitX, int hitY, int playerDist) 
    : id(id), type(type), hitX(hitX), hitY(hitY), playerDist(playerDist){}

    bool operator<(const Target& rhs) const {
        return playerDist > rhs.playerDist;
    }

    int32_t getId() const {return id;}
    int getType() const {return type;}
    int getHitX() const {return hitX;}
    int getHitY() const {return hitY;}
    bool isType(int check){return check == type;}

private:
    int32_t id;
    int type;
    int hitX;
    int hitY;
    int playerDist;
};


/**
    class TargetList

    DISCRIPTION:
        This is used to store all the targets allong the weapons firing path.
        Targets will be sorted by distance from origin.

    PARAMATERS:
        originX and originY represent the muzzle of the gun.
        
        endX and endY represent the point where the bullet will hit the ground if it hits nothing else.

    AUTHOR: Deric Mccadden 3/29/2017

*/
class TargetList{
public:

    void addTarget(const Target t){targets.push(t);}
    const Target& getNextTarget() {return targets.top();}
    void removeTop(){targets.pop();}
    bool isEmpty(){return targets.empty();}
    int numTargets(){return targets.size();}

    int getOriginX() const {return originX;}
    int getOriginY() const {return originY;}
    void setOriginX(const int x) {originX = x;}
    void setOriginY(const int y) {originY = y;}
    int getEndX() const {return endX;}
    int getEndY() const {return endY;}
    void setEndX(const int x) {endX = x;}
    void setEndY(const int y) {endY = y;}
    
private:
    std::priority_queue<Target> targets;
    int originX;
    int originY;
    int endX;
    int endY;
};


#endif
