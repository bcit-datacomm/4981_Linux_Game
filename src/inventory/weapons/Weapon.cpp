/*
    Created by Maitiu Morton 2/1/2017
        Edited by DericM 3/8/2017
*/
#include "Weapon.h"
#include <iostream>
#include <atomic>

#include "../../log/log.h"
#include "../../audio/AudioManager.h"

using std::string;

Weapon::Weapon(const string &type, const string &fireSound, const string &hitSound, const string &reloadSound,
        const string &emptySound, const int range, const int damage, const int AOE, const int penetration,
        const int clip, const int clipMax, const int ammo, const int reloadDelay, const int fireDelay)
: type(type), fireSound(fireSound), hitSound(hitSound), reloadSound(reloadSound), emptySound(emptySound),
        range(range), damage(damage), AOE(AOE), penetration(penetration), clip(clip), clipMax(clipMax), ammo(ammo),
        reloadDelay(reloadDelay), fireDelay(fireDelay), reloadTick(0), fireTick(0),  wID(generateWID()){

}

Weapon::Weapon(const Weapon &w)
: type(w.type), fireSound(w.fireSound), hitSound(w.hitSound), reloadSound(w.reloadSound), emptySound(w.emptySound),
        range(w.range), damage(w.damage), AOE(w.AOE), penetration(w.penetration), clip(w.clip), clipMax(w.clipMax),
        ammo(w.ammo), reloadDelay(w.reloadDelay), fireDelay(w.fireDelay), reloadTick(w.reloadTick),
        fireTick(w.fireTick), wID(w.getId()){
}


//Deric M       3/3/2017
bool Weapon::reduceClip(const int rounds){
    logv("Current ammo: %d/%d\n", clip, ammo + clip);
    if(clip < rounds){
        reloadClip();
    }
    clip -= rounds;
    return true;
}


//Mark T    3/8/2017
//Deric M       3/15/2017
bool Weapon::reloadClip(){
    int currentTime = SDL_GetTicks();
    if(currentTime < (reloadTick + reloadDelay)){
        return false;
    }
    reloadTick = currentTime;
    fireTick += reloadDelay; //must wait extra time to fire from reloading
    if(ammo <= 0){
        AudioManager::instance().playEffect(emptySound.c_str());
        return false;
    }

    int ammoNeeded = clipMax - clip;
    if(ammo < ammoNeeded){
        clip += ammo;
        ammo = 0;
        return true;
    }
    ammo -= ammoNeeded;
    clip += ammoNeeded;
    AudioManager::instance().playEffect(reloadSound.c_str());

    return true;
}


//Mark T    3/8/2017
//Deric M       3/15/2017
bool Weapon::chamberRound() {
    int currentTime = SDL_GetTicks();
    if(currentTime < (fireTick + fireDelay)){
        return false;
    }
    fireTick = currentTime;
    if(!reduceClip(1)){
        return false;
    }
    return true;
}


int32_t generateWID() {
    static std::atomic<int32_t> counter{-1};
    return ++counter;
}


//Deric M       3/15/2017
bool Weapon::fire(Marine &marine){
    logv("Weapon::fire()\n");
    if(!chamberRound()){
        return false;
    }
    AudioManager::instance().playEffect(fireSound.c_str());

    return true;
}
