/*
    Created by Maitiu Morton 2/8/2017
        Edited by DericM 3/8/2017
        Edited by MarkT 3/13/2017
*/
#include "HandGun.h"
<<<<<<< HEAD
/**
 * Date: Feb 8, 2017
 * Modified: Mar 13, 2017 - Mark Tattrie
 * Author: Deric Mccadden
 * Function Interface: HandGun::HandGun() : InstantWeapon(HandgunVars::TYPE, HandgunVars::FIRESOUND,
 *       HandgunVars::HITSOUND, HandgunVars::RELOADSOUND, HandgunVars::EMPTYSOUND, HandgunVars::RANGE,
 *       HandgunVars::DAMAGE, HandgunVars::AOE, HandgunVars::PENETRATION, HandgunVars::CLIP,
 *       HandgunVars::CLIPMAX, HandgunVars::AMMO, HandgunVars::RELOADDELAY, HandgunVars::FIREDELAY)
 * Description:
 * Ctor for HandGun
 */
HandGun::HandGun() : InstantWeapon(HandgunVars::TYPE, HandgunVars::FIRESOUND, HandgunVars::HITSOUND,
        HandgunVars::RELOADSOUND, HandgunVars::EMPTYSOUND, HandgunVars::RANGE, HandgunVars::DAMAGE,
        HandgunVars::AOE, HandgunVars::PENETRATION, HandgunVars::CLIP, HandgunVars::CLIPMAX, HandgunVars::AMMO,
        HandgunVars::RELOADDELAY, HandgunVars::FIREDELAY) {
=======

/*
 * Created by Maitiu
 * Edited by Deric
 */
HandGun::HandGun(int32_t id) : InstantWeapon(HandgunVars::TYPE, TEXTURES::SHOTGUN, HandgunVars::FIRESOUND, HandgunVars::HITSOUND,
        HandgunVars::RELOADSOUND, HandgunVars::EMPTYSOUND, HandgunVars::RANGE, HandgunVars::DAMAGE,
        HandgunVars::AOE, HandgunVars::PENETRATION, HandgunVars::CLIP, HandgunVars::CLIPMAX, HandgunVars::AMMO,
        HandgunVars::RELOADDELAY, HandgunVars::FIREDELAY, id) {
>>>>>>> 458c5f50b3a19eda537f89c4613db4a17709ffc0

}
