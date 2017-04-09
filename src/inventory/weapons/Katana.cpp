/*
    Created by Deric Mccadden Apr/6/2017
*/
#include "Katana.h"

/**
 * Created by Deric Mccadden Apr/6/2017
 * Function Interface: Katana::Katana() : InstantWeapon(KatanaVars::TYPE, KatanaVars::FIRESOUND,
 *       KatanaVars::HITSOUND, KatanaVars::RELOADSOUND, KatanaVars::EMPTYSOUND, KatanaVars::RANGE,
 *       KatanaVars::DAMAGE, KatanaVars::AOE, KatanaVars::PENETRATION, KatanaVars::CLIP,
 *       KatanaVars::CLIPMAX, KatanaVars::AMMO, KatanaVars::RELOADDELAY, KatanaVars::FIREDELAY, id)
 * Description:
 * Ctor for Katana
 */
Katana::Katana(const int32_t id) : InstantWeapon(KatanaVars::TYPE, TEXTURES::SHOTGUN, KatanaVars::FIRESOUND,
        KatanaVars::HITSOUND, KatanaVars::RELOADSOUND, KatanaVars::EMPTYSOUND, KatanaVars::RANGE,
        KatanaVars::DAMAGE, KatanaVars::AOE, KatanaVars::PENETRATION, KatanaVars::ACCURACY, KatanaVars::CLIP,
        KatanaVars::CLIPMAX, KatanaVars::AMMO, KatanaVars::RELOADDELAY, KatanaVars::FIREDELAY, id) {

}
