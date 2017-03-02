#include "Weapon.h"

Weapon::Weapon(std::string type, int range, int damage, int clip, int clipMax, int ammo,int rAOE, int reloadSpeed)
               :type(type), range(range), damage(damage), ammo(ammo), rAOE(rAOE), reloadSpeed(reloadSpeed),
               reloadTick(0), reloadDelay(200){

}


Weapon::~Weapon(){

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

    if(currentTime > (reloadTick + reloadDelay)){
        reloadTick = currentTime;
        printf("RELOADED\n");
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

void Weapon::shot(){
    ammo--;
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
