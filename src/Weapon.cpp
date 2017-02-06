#include "Weapon.h"

using namespace std;

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
    int nextLoad;
    if(ammo >= clipMax){
        clip = clipMax;
    } if (ammo < clipMax){
        if((nextLoad = clip + ammo) =< clipMax){
            clip = nextLoad;
        } else {
            clip = clipMax;
        }
    }
}

void Weapon::shot(){
    ammo--;
}

int Weapon::getClipMax(){
    return clipMax;
}
