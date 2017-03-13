/*
    Created by Maitiu Morton 2/8/2017
        Edited by DericM 3/8/2017
*/
#include "HandGun.h"

HandGun::HandGun() : InstantWeapon(HandgunVars::TYPE, HandgunVars::RANGE, HandgunVars::DAMAGE,
        HandgunVars::CLIP, HandgunVars::CLIPMAX, HandgunVars::AMMO, HandgunVars::AOE, 
        HandgunVars::RELOAD, HandgunVars::FIRERATE, HandgunVars::READY) {

}
