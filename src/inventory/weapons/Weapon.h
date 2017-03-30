/*
    Created by Maitiu Morton 2/1/2017
        Edited by DericM 3/8/2017
*/
#ifndef WEAPON_H
#define WEAPON_H

#include <SDL2/SDL.h>
#include <string>

using std::string;

class Movable;

class Weapon {
public:

    Weapon(const string& type = "no type", const string& fireSound = "", const string& hitSound = "",
        const string& reloadSound = "", const string& emptySound = "", const int range = 0,
        const int damage = 0, const int AOE = 0, const int penetration = 0, const int clip = 0,
        const int clipMax = 0, const int ammo = 0, const int reloadDelay = 0, const int fireDelay = 0);

    Weapon(const Weapon& w);
    ~Weapon() = default;

    string getType() const { return type; }

    int getRange() const { return range; }
    int getDamage() const { return damage; }
    int getAOE() const { return AOE; }

    int getClip() const { return clip; }
    int getClipMax() const { return clipMax; }
    int getAmmo() const { return ammo; }

    int getReloadSpeed() const { return reloadDelay; }
    int getFireRate() const { return fireDelay; }

    bool reduceClip(const int rounds);
    bool reloadClip();
    bool chamberRound();

    int32_t getId()const{return wID;};
    void setId(const int32_t num){wID = num;};

    virtual bool fire(Movable& movable);

protected:
    string type;

    string fireSound;
    string hitSound;
    string reloadSound;
    string emptySound;

    int range;
    int damage;
    int AOE;
    int penetration;

    int clip;
    int clipMax;
    int ammo;
    int reloadDelay;
    int fireDelay;

    int reloadTick;
    int fireTick;

    int32_t wID;

};

int32_t generateWID();

#endif
