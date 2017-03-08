/*
    Created by Maitiu Morton 2/8/2017
*/
#ifndef RIFLE_H
#define RIFLE_H
#include "InstantWeapon.h"
#include <string>

using std::string;

namespace RifleVars {
    const string TYPE  = "Rifle";
    const int RANGE    = 500;
    const int DAMAGE   = 200;
    const int CLIP     = 2;
    const int CLIPMAX  = 2;
    const int AMMO     = 1000;
    const int AOE      = 1;
    const int RELOAD   = 3;
}

class Rifle: public InstantWeapon {
public:

    Rifle();
    ~Rifle();
};

#endif
