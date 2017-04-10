/*------------------------------------------------------------------------------
* Source: Weapon.h      
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
    Created by Maitiu Morton 2/1/2017
        Edited by DericM 3/8/2017
        Edited by MarkT 3/13/2017
        Edited by Isaac Morneau 4/10/2017
*/
#ifndef WEAPON_H
#define WEAPON_H

#include <SDL2/SDL.h>
#include <string>
#include "../../sprites/SpriteTypes.h"

//taken from the spritesheet
static constexpr int WEAPON_WIDTH = 256;
static constexpr int WEAPON_HEIGHT = 180;


using std::string;

class Movable;

class Weapon {
public:

    Weapon(const string& type = "no type", TEXTURES sprite = TEXTURES::CONCRETE, const string& fireSound = "", const string& hitSound = "",
        const string& reloadSound = "", const string& emptySound = "", const int range = 0,
        const int damage = 0, const int AOE = 0, const int penetration = 0, const int accuracy = 0, const int clip = 0,
        const int clipMax = 0, const int ammo = 0, const int reloadDelay = 0, const int fireDelay = 0, const int texX = 0, 
        const int texY = 0, int32_t id = -1);

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

    TEXTURES getTexture() const{ return spriteType;};

    bool reduceClip(const int rounds);
    bool reloadClip();
    bool chamberRound();

    int32_t getID() const {return wID;};

    virtual bool fire(Movable& movable);

    virtual void updateGunRender(const Movable& mov, const SDL_Rect& camera);

protected:

    SDL_Rect weaponSrc;
    SDL_Rect weaponDest;
    SDL_Point rotate;

    string type;

    TEXTURES spriteType;
    string fireSound;
    string hitSound;
    string reloadSound;
    string emptySound;

    int range;
    int damage;
    int AOE;
    int penetration;
    int accuracy;

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
