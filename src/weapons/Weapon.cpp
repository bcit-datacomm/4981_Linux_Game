/*
    Created by Maitiu Morton 2/1/2017
        Edited by DericM 3/8/2017
*/
#include "Weapon.h"
#include <iostream>
#include <atomic>

Weapon::Weapon(std::string type, int range, int damage, int clip, int clipMax, int ammo,int rAOE,
        int reloadSpeed, int fireRate, bool isReadyToFire): type(type), range(range), damage(damage),
        ammo(ammo), rAOE(rAOE), reloadSpeed(reloadSpeed), reloadTick(0), reloadDelay(200),
        fireRate(fireRate), fireTick(0), isReadyToFire(isReadyToFire), wID(generateWID()){

}

Weapon::Weapon(const Weapon& w) : type(w.type), range(w.range), damage(w.damage), ammo(w.ammo),
            rAOE(w.rAOE), reloadSpeed(w.reloadSpeed), reloadTick(w.reloadTick), reloadDelay(w.reloadDelay),
            fireRate(w.fireRate), fireTick(w.fireTick), isReadyToFire(w.isReadyToFire), wID(w.getId()){
}


int Weapon::getClip(){
    return clip;
}

int Weapon::getDamage(){
    return damage;
}

int Weapon::getRange(){
    return range;
}

void Weapon::reloadClip(){

    int currentTime = SDL_GetTicks();


    printf("currentTime: %d, reloadTick: %d, reloadDelay: %d fireTick: %d\n", currentTime, reloadTick, reloadDelay, fireTick);

    if(currentTime > (reloadTick + reloadDelay)){
        reloadTick = currentTime;
        fireTick += reloadDelay; //must wait extra time to fire from reloading
        printf("RELOADED\n");
        isReadyToFire = true;
        ammo = 50; //for testing isReadyToFire
        /*if(ammo >= clipMax){
            clip = clipMax;
        } if (ammo < clipMax){
            int nextLoad = clip + ammo;
            if(nextLoad <= clipMax){
                clip = nextLoad;
            } else {
                clip = clipMax;
            }
        }*/
    }

}

int Weapon::getClipMax(){
    return clipMax;
}

std::string Weapon::getType(){
    return type;
}

int Weapon::getReloadSpeed(){
    return reloadSpeed;
}

int Weapon::getAmmo(){
    return ammo;
}

int Weapon::getRAOE(){
    return rAOE;
}

//Deric M       3/3/2017
bool Weapon::reduceAmmo(int rounds){
    std::cout << "Current ammo:" << ammo << std::endl;
    if(ammo < rounds){
        return false;
    }
    ammo -= rounds;
    return true;
}

int Weapon::getFireRate(){
    return fireRate;
}

//Mark T    3/8/2017
bool Weapon::getFireState(){

    int currentTime = SDL_GetTicks();

    if (ammo == 0){
        isReadyToFire = false;
        printf("isReadyToFire = false, ammo = 0\n");
    }

    //  printf("currentTime: %d, fireTick: %d, fireRate: %d\n", currentTime, fireTick, fireRate);
    if(currentTime > (fireTick + fireRate)){
        fireTick = currentTime;
        isReadyToFire = true;
    }else{
        isReadyToFire = false;
    }
    return isReadyToFire;
}

void Weapon::fire(Marine &marine){

}

int32_t generateWID() {
    static std::atomic<int32_t> counter{-1};
    return ++counter;
}
