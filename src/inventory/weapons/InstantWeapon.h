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
    InstantWeapon(string type, TEXTURES sprite, string fireSound, string hitSound, string reloadSound, string emptySound,
            int range, int damage, int AOE, int penetration, int accuracy, int clip, int clipMax, int ammo, int reloadDelay,
            int fireDelay, int32_t id);
    ~InstantWeapon() = default;

    virtual bool fire(Movable& movable);
    void fireSingleProjectile(int gunX, int gunY, double angle);

    virtual void fireAnimation(const int gunX, const int gunY, const int endX, const int endY);

};

#endif
