/*------------------------------------------------------------------------------
* Source: HandGun.cpp         
*
* Functions:
*
* Date: 
*
* Revisions: 
* Edited By : Tim Makimov on 2017/APR/10
*
* Designer: 
*
* Author: 
*
* Notes:
------------------------------------------------------------------------------*/

/*
    Created by Maitiu Morton 2/8/2017
        Edited by DericM 3/8/2017
        Edited by MarkT 3/13/2017
*/
#include "HandGun.h"

/**
 * Date: Feb 8, 2017
 * Modified:    Mar 8, 2017 - Deric Mccadden
 *              Mar 13, 2017 - Mark Tattrie
 * Author: Maitiu Morton
 * Function Interface: HandGun::HandGun() : InstantWeapon(HandgunVars::TYPE, HandgunVars::FIRESOUND,
 *       HandgunVars::HITSOUND, HandgunVars::RELOADSOUND, HandgunVars::EMPTYSOUND, HandgunVars::RANGE,
 *       HandgunVars::DAMAGE, HandgunVars::AOE, HandgunVars::PENETRATION, HandgunVars::CLIP,
 *       HandgunVars::CLIPMAX, HandgunVars::AMMO, HandgunVars::RELOADDELAY, HandgunVars::FIREDELAY, id)
 * Description:
 * Ctor for HandGun
 */
HandGun::HandGun(const int32_t id) : InstantWeapon(HandgunVars::TYPE, TEXTURES::SHOTGUN, HandgunVars::FIRESOUND,
        HandgunVars::HITSOUND, HandgunVars::RELOADSOUND, HandgunVars::EMPTYSOUND, HandgunVars::RANGE,
        HandgunVars::DAMAGE, HandgunVars::AOE, HandgunVars::PENETRATION, HandgunVars::ACCURACY, HandgunVars::CLIP,
        HandgunVars::CLIPMAX, HandgunVars::AMMO, HandgunVars::RELOADDELAY, HandgunVars::FIREDELAY, HandgunVars::START_X,
        HandgunVars::START_Y, id) {

}
