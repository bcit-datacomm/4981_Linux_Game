#include "Weapon.h"

Weapon::Weapon(std::string type, 
			   int range, 
			   int damage, 
			   int clip, 
			   int clipMax, 
			   int ammo, 
			   int rAOE, 
			   int reloadSpeed){
	this->type = type;
	this->range = range;
	this->damage = damage;
	this->clip = clip;
	this->clipMax = clipMax;
	this->ammo = ammo;
	this->rAOE = rAOE;
	this->reloadSpeed = reloadSpeed;
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

std::string Weapon::getType(){
    return type;
}

int Weapon::getReloadSpeed(){
    return reloadSpeed;
}
