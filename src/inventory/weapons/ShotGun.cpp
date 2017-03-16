/*
    Created by Maitiu Morton 2/8/2017
        Edited by DericM 3/8/2017
*/
#include "ShotGun.h"

ShotGun::ShotGun() : InstantWeapon(ShotgunVars::TYPE, 
    ShotgunVars::FIRESOUND, ShotgunVars::HITSOUND, 
    ShotgunVars::RELOADSOUND, ShotgunVars::EMPTYSOUND, 
    ShotgunVars::RANGE, ShotgunVars::DAMAGE, ShotgunVars::AOE,
    ShotgunVars::CLIP, ShotgunVars::CLIPMAX, ShotgunVars::AMMO,  
    ShotgunVars::RELOADDELAY, ShotgunVars::FIREDELAY) {

}


/*
bool ShotGun::fire(Marine &marine){
    if(Weapon::fire(marine)){
        return false;
    }

}
*/