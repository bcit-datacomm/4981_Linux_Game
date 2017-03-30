/*
    Created by Maitiu Morton 2/8/2017
        Edited by DericM 3/8/2017
*/
#include "HandGun.h"

/*
 * Created by Maitiu
 * Edited by Deric
 */
HandGun::HandGun(int32_t id) : InstantWeapon(HandgunVars::TYPE, TEXTURES::SHOTGUN, HandgunVars::FIRESOUND, HandgunVars::HITSOUND,
        HandgunVars::RELOADSOUND, HandgunVars::EMPTYSOUND, HandgunVars::RANGE, HandgunVars::DAMAGE,
        HandgunVars::AOE, HandgunVars::PENETRATION, HandgunVars::CLIP, HandgunVars::CLIPMAX, HandgunVars::AMMO,
        HandgunVars::RELOADDELAY, HandgunVars::FIREDELAY, id) {

}
