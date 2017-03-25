/*
    Created by Maitiu Morton 2/8/2017
*/
#ifndef RIFLE_H
#define RIFLE_H
#include "InstantWeapon.h"
#include <string>

using std::string;

namespace RifleVars {
    const string TYPE = "Rifle";
    constexpr int RANGE = 800;
    constexpr int DAMAGE = 200;
    constexpr int CLIP = 2;
    constexpr int CLIPMAX = 2;
    constexpr int AMMO = 1000;
    constexpr int AOE = 1;
    constexpr int RELOAD = 3;
    constexpr int FIRERATE = 1000;
    constexpr bool READY = true;
}

class Rifle: public InstantWeapon {
public:

    Rifle();
    ~Rifle() = default;
};

#endif
