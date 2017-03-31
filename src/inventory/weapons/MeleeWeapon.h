#ifndef MELEEWEAPON_H
#define MELEEWEAPON_H
#include "Weapon.h"
#include <vector>
#include "../../collision/CollisionHandler.h"

#define Z_WIDTH 75
#define Z_HEIGHT 125

class MeleeWeapon: public Weapon {
public:
    MeleeWeapon(string type, TEXTURES sprite, string fireSound, string hitSound, string reloadSound,
            string emptySound, int range, int damage, int AOE, int penetration, int clip,
            int clipMax, int ammo, int reloadDelay, int fireDelay, int32_t id);
    ~MeleeWeapon() = default;

    virtual bool fire(Movable &marine);
    //bool fire(Zombie &zombie);


protected:

};

#endif
