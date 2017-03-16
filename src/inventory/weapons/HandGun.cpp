/*
    Created by Maitiu Morton 2/8/2017
        Edited by DericM 3/8/2017
*/
#include "HandGun.h"

HandGun::HandGun() : InstantWeapon(HandgunVars::TYPE, HandgunVars::FIRESOUND, HandgunVars::HITSOUND, 
    HandgunVars::RELOADSOUND, HandgunVars::EMPTYSOUND, HandgunVars::RANGE, HandgunVars::DAMAGE, 
    HandgunVars::AOE, HandgunVars::CLIP, HandgunVars::CLIPMAX, HandgunVars::AMMO, HandgunVars::RELOADDELAY, 
    HandgunVars::FIREDELAY) {

}
