/*
    Created by Maitiu Morton 2/1/2017
*/
#ifndef WEAPON_H
#define WEAPON_H
#include <SDL2/SDL.h>
#include "LTexture.h"
#include <string>


class Weapon {
public:

    int getClip(); //returns bullets in clip
    int getDamage(); //returns damage of weapon
    int getRange(); //returns range of weapon
    void reloadClip();//resets clip to max amount
    void shot();//decreases the amount of ammo
    int getClipMax(); //returns max amount clip can hold
    std::string getType();
    int getReloadSpeed();

	Weapon();
	~Weapon();

protected:
    std::string type = "no type";
    int range; //range of weapon
    int damage;//damage per bullet
    int clip;//bullets in clip
    int clipMax;//max amount clip can hold
    int ammo; //amount of bullets total
    int rAOE; //radius of area of effect
    int reloadSpeed;
    LTexture inGameTexture;
    LTexture UiTexture;

};

#endif
