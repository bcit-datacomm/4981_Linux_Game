#include "Weapon.h"

using namespace std;

Weapon::Weapon(){

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

    if(ammo >= clipMax){
        clip = clipMax;
    } if (ammo < clipMax){
        int nextLoad = clip + ammo;
        if(nextLoad <= clipMax){
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

string Weapon::getType(){
    return type;
}

int Weapon::getReloadSpeed(){
    return reloadSpeed;
}
