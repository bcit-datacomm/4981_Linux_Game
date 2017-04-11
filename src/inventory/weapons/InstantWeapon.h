/*------------------------------------------------------------------------------
* Source: InstantWeapon.h        
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

/*
    Created  by Deric M       01/03/17
    Edited by Mark T 3/13/2017
*/
#ifndef INSTANTWEAPON_H
#define INSTANTWEAPON_H
#include "Weapon.h"
#include "../../collision/CollisionHandler.h"
#include "Target.h"
#include "../../sprites/SpriteTypes.h"

using std::string;

class InstantWeapon: public Weapon  {
public:
    InstantWeapon(const string& type, TEXTURES sprite, const string& fireSound, const string& hitSound, const string& reloadSound,
            const string& emptySound, const int range, const int damage, const int AOE, const int penetration, const int accuracy,
            const int clip, const int clipMax, const int ammo, const int reloadDelay, const int fireDelay, const int texX,
            const int texY, const int32_t id);
    ~InstantWeapon() = default;

    virtual bool fire(Movable& movable);
    void fireSingleProjectile(const int gunX, const int gunY, const double angle);

    virtual void fireAnimation(const int gunX, const int gunY, const int endX, const int endY);

};

#endif
