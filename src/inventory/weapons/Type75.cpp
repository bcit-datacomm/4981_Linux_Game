/*
    Created by Deric Mccadden Apr/6/2017
*/
#include "Type75.h"

/**
 * Created by Deric Mccadden Apr/6/2017
 * Function Interface: Type75::Type75(int32_t id) : InstantWeapon(Type75Vars::TYPE, Type75Vars::FIRESOUND,
 *       Type75Vars::HITSOUND, Type75Vars::RELOADSOUND, Type75Vars::EMPTYSOUND, Type75Vars::RANGE,
 *       Type75Vars::DAMAGE, Type75Vars::AOE, Type75Vars::PENETRATION, Type75Vars::CLIP, Type75Vars::CLIPMAX,
 *       Type75Vars::AMMO, Type75Vars::RELOADDELAY, Type75Vars::FIREDELAY, id
 * Description:
 * Ctor for Rifle
 */

Type75::Type75(const int32_t id) : InstantWeapon(Type75Vars::TYPE, TEXTURES::RIFLE, Type75Vars::FIRESOUND, 
		Type75Vars::HITSOUND, Type75Vars::RELOADSOUND, Type75Vars::EMPTYSOUND, Type75Vars::RANGE, Type75Vars::DAMAGE, 
		Type75Vars::AOE, Type75Vars::PENETRATION, Type75Vars::ACCURACY, Type75Vars::CLIP, Type75Vars::CLIPMAX, 
		Type75Vars::AMMO, Type75Vars::RELOADDELAY, Type75Vars::FIREDELAY, id) {

}
