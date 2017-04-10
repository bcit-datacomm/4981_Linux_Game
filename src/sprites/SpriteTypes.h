#ifndef SPRITETYPES_H
#define SPRITETYPES_H

/* DEVELOPER: Michael Goll
** DESIGNER:  Michael Goll
** DATE:      March 14, 2017
*/

#include <cstdlib>

/**
* NOTE:
* If you change any of the sprites (ex. adding a new sprite),
* you MUST change the TOTAL_SPRITES value in Renderer.h accordingly.
*
* ADDING A NEW SPRITE STEPS:
* 1) Add Enum to list below
* 2) Update the TOTAL_SPRITES value in Renderer.h
* 3) Add the new texture's path to Renerer.h as a const string above the class
* 4) Call the createTexture() function inside the loadSprites() function in Render.cpp.
*
* Modified by:
* Jacob Frank (March 29, 2017)
*
* Revisions:
* JF Mar 25/29: Added Enums for Menu and HUD textures
*/

enum class TEXTURES : int {

    //--------------------- Menu textures ---------------------
    //Main Menu
    MAIN, //Splash screen
    TEXTBOX,
    TEXTBOX_ACTIVE,
    TEXTBOX_TRANSPARENT,
    JOIN_FONT,
    JOIN_FONT_ACTIVE,

    //--------------------- Hud textures ---------------------
    WEAPON_CLIP_FULL,
    WEAPON_CLIP_EMPTY,
    ACTIVE_SLOT,
    PASSIVE_SLOT,
    HEALTHBAR,
    CONSUMABLE_SLOT,
    EQUIPPED_WEAPON_SLOT,

    //--------------------- Map textures ---------------------
    BARREN,
    DEAD_GRASS,
    TERRAFORMED,
    CONCRETE,

    //--------------------- Map Object textures ---------------------

    MAP_OBJECTS,
    BASE,
    //---------- Nature ----------
    TURRET,
    HEALTHPACK,

    //--------------------- Weapons ---------------------

    WEAPONS,

    RIFLE,
    SHOTGUN,
    HANDGUN,
    PLASMA_DEAGLE,
    RAILGUN,
    RPG,
    TYPE_75K,
    KATANA,
    PHASE_REPEATER,

    RIFLE_INVENTORY,
    SHOTGUN_INVENTORY,
    HANDGUN_INVENTORY,
    PLASMA_DEAGLE_INVENTORY,
    RAILGUN_INVENTORY,
    RPG_INVENTORY,
    TYPE_75K_INVENTORY,
    KATANA_INVENTORY,
    PHASE_REPEATER_INVENTORY,
    //--------------------- Marine textures ---------------------

    MARINE, //Sprite sheet, not chunked up
    COWBOY, //Sprite sheet, not chunked up
    //--------------------- Zombie textures ---------------------

    BLOOD,
    //---------- Baby Zombie ----------
    BABY_ZOMBIE, //Sprite sheet, not split

    //---------- Digger Zombie ----------
    DIGGER_ZOMBIE,

    //---------- Boss Zombie ----------
    BOSS_ZOMBIE //Sprite sheet, not split
};

#endif
