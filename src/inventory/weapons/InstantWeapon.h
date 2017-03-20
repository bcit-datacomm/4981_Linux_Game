/*
    Created  by Deric M       01/03/17
*/
#ifndef INSTANTWEAPON_H
#define INSTANTWEAPON_H
#include "Weapon.h"
#include "../../collision/CollisionHandler.h"
#include "Target.h"

using std::string;

class InstantWeapon: public Weapon  {
public:
    InstantWeapon(string type, string fireSound, string hitSound, string reloadSound, string emptySound,
	        int range, int damage, int AOE, int penetration, int clip, int clipMax, int ammo, int reloadDelay, 
	        int fireDelay);
    ~InstantWeapon() = default;

    virtual bool fire(Marine &marine);

};

#endif
