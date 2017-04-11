/*------------------------------------------------------------------------------
* Source: MeleeWeapon.h        
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

#ifndef MELEEWEAPON_H
#define MELEEWEAPON_H
#include "Weapon.h"
#include <vector>
#include "../../collision/CollisionHandler.h"

#define Z_WIDTH 75
#define Z_HEIGHT 125

class MeleeWeapon: public Weapon {
public:
    MeleeWeapon(const string& type, TEXTURES sprite, const string& fireSound, const string& hitSound,
            const string& reloadSound, const string& emptySound, const int range, const int damage,
            const int AOE, const int penetration, const int accuracy, const int clip, const int clipMax, const int ammo,
            const int reloadDelay, const int fireDelay, const int texX, const int texY, int32_t id);
    ~MeleeWeapon() = default;

    virtual bool fire(Movable &marine);
    //bool fire(Zombie &zombie);


protected:

};

#endif
