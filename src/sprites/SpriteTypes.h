#ifndef SPRITETYPES_H
#define SPRITETYPES_H

/* DEVELOPER: Michael Goll
** DESIGNER:  Michael Goll
** DATE:      March 14, 2017
*/

#include <stdlib.h>

/**
*   NOTE: If you change any of the sprites (ex. adding a new sprite), you
*         MUST change the TOTAL_SPRITES value in Renderer.h accordingly
*
*/

enum class TEXTURES : int {

    //--------------------- Menu textures ---------------------
    //Main Menu
    MAIN, //Splash screen
    TEXTBOX,
    JOIN_FONT,
    OPTIONS_FONT,

    //LOBBY,


    //--------------------- Map textures ---------------------
    BARREN,
    DEAD_GRASS,
    TERRAFORMED,
    CONCRETE,

    //--------------------- Map Object textures ---------------------

    MAP_OBJECTS,
    //---------- Nature ----------
    // TREE,
    // ROCK,
    // SHRUB,

    // ---------- Consumables ----------
    // BARRICADE_FULL_HEALTH,
    // BARRICADE_NOT_QUITE_FULL,
    // BARRICADE_HALF_HEALTH,
    // BARRICADE_ALMOST_DESTROYED,
    // TURRET,

    //---------- Shops ----------
    // SHOP_GUNS,         //Guns, ammo, etc
    // SHOP_HEALTH,
    // SHOP_CONSUMABLES, //Turrets, walls, etc

    //--------------------- Weapons ---------------------

    WEAPONS,
    //Pistol
    // PISTOL_FRONT,
    // PISTOL_FRONT_LEFT,
    // PISTOL_FRONT_RIGHT,
    // PISTOL_LEFT,
    // PISTOL_RIGHT,
    // PISTOL_BACK_RIGHT,
    // PISTOL_BACK_LEFT,
    // PISTOL_BACK,
    //
    // //Rocket Launcher
    // ROCKET_LAUNCHER_FRONT,
    // ROCKET_LAUNCHER_FRONT_LEFT,
    // ROCKET_LAUNCHER_FRONT_RIGHT,
    // ROCKET_LAUNCHER_LEFT,
    // ROCKET_LAUNCHER_RIGHT,
    // ROCKET_LAUNCHER_BACK_RIGHT,
    // ROCKET_LAUNCHER_BACK_LEFT,
    // ROCKET_LAUNCHER_BACK,
    //
    // //Assaul Rifle
    // AR_FRONT,
    // AR_FRONT_LEFT,
    // AR_FRONT_RIGHT,
    // AR_LEFT,
    // AR_RIGHT,
    // AR_BACK_RIGHT,
    // AR_BACK_LEFT,
    // AR_BACK,
    //
    // //Lazer Pistol
    // LAZER_PISTOL_FRONT,
    // LAZER_PISTOL_FRONT_LEFT,
    // LAZER_PISTOL_FRONT_RIGHT,
    // LAZER_PISTOL_LEFT,
    // LAZER_PISTOL_RIGHT,
    // LAZER_PISTOL_BACK_RIGHT,
    // LAZER_PISTOL_BACK_LEFT,
    // LAZER_PISTOL_BACK,
    //
    // //ShotGun
    // SHOTGUN_FRONT,
    // SHOTGUN_FRONT_LEFT,
    // SHOTGUN_FRONT_RIGHT,
    // SHOTGUN_LEFT,
    // SHOTGUN_RIGHT,
    // SHOTGUN_BACK_RIGHT,
    // SHOTGUN_BACK_LEFT,
    // SHOTGUN_BACK,
    //
    // //Double Barrel ShotGun
    // DB_SHOTGUN_FRONT,
    // DB_SHOTGUN_FRONT_LEFT,
    // DB_SHOTGUN_FRONT_RIGHT,
    // DB_SHOTGUN_LEFT,
    // DB_SHOTGUN_RIGHT,
    // DB_SHOTGUN_BACK_RIGHT,
    // DB_SHOTGUN_BACK_LEFT,
    // DB_SHOTGUN_BACK,
    //
    // //Lazer Rocket Launcher
    // LAZER_ROCKET_LAUNCHER_FRONT,
    // LAZER_ROCKET_LAUNCHER_FRONT_LEFT,
    // LAZER_ROCKET_LAUNCHER_FRONT_RIGHT,
    // LAZER_ROCKET_LAUNCHER_LEFT,
    // LAZER_ROCKET_LAUNCHER_RIGHT,
    // LAZER_ROCKET_LAUNCHER_BACK_RIGHT,
    // LAZER_ROCKET_LAUNCHER_BACK_LEFT,
    // LAZER_ROCKET_LAUNCHER_BACK,

    //--------------------- Marine textures ---------------------

    MARINE, //Sprite sheet, not chunked up
    //Marine Front
    // MARINE_FRONT_IDLE,
    // MARINE_FRONT_STEP_RIGHT,
    // MARINE_FRONT_STEP_LEFT,
    // MARINE_FRONT_DAMAGED,
    //
    // //Marine 45 Degrees Left
    // MARINE_FRONT_LEFT_IDLE,
    // MARINE_FRONT_LEFT_STEP_RIGHT,
    // MARINE_FRONT_LEFT_STEP_LEFT,
    // MARINE_FRONT_LEFT_DAMAGED,
    //
    // //Marine Left Side
    // MARINE_LEFT_IDLE,
    // MARINE_LEFT_STEP_RIGHT,
    // MARINE_LEFT_STEP_LEFT,
    // MARINE_LEFT_DAMAGED,
    //
    // //Marine 45 Degrees Back Right
    // MARINE_BACK_RIGHT_IDLE,
    // MARINE_BACK_RIGHT_STEP_RIGHT,
    // MARINE_BACK_RIGHT_STEP_LEFT,
    // MARINE_BACK_RIGHT_DAMAGED,
    //
    // //Marine Back
    // MARINE_BACK_IDLE,
    // MARINE_BACK_RIGHT,
    // MARINE_BACK_LEFT,
    // MARINE_BACK_DAMAGED,
    //
    // //Marine 45 Degrees Back Left
    // MARINE_BACK_LEFT_IDLE,
    // MARINE_BACK_LEFT_STEP_RIGHT,
    // MARINE_BACK_LEFT_STEP_LEFT,
    // MARINE_BACK_LEFT_DAMAGED,
    //
    // //Marine Right Side
    // MARINE_RIGHT_IDLE,
    // MARINE_RIGHT_STEP_RIGHT,
    // MARINE_RIGHT_STEP_LEFT,
    // MARINE_RIGHT_DAMAGED,
    //
    // //Marine 45 Degrees Right
    // MARINE_FRONT_RIGHT_IDLE,
    // MARINE_FRONT_RIGHT_STEP_RIGHT,
    // MARINE_FRONT_RIGHT_STEP_LEFT,
    // MARINE_FRONT_RIGHT_DAMAGED,

    //--------------------- Zombie textures ---------------------

    //---------- Baby Zombie ----------
    BABY_ZOMBIE, //Sprite sheet, not split
    //Baby Zombie Front
    // BABY_ZOMBIE_FRONT_IDLE,
    // BABY_ZOMBIE_FRONT_STEP_LEFT,
    // BABY_ZOMBIE_FRONT_STEP_RIGHT,
    // BABY_ZOMBIE_FRONT_DAMAGED,
    // BABY_ZOMBIE_FRONT_ATTACK,
    //
    // //Baby Zombie 45 Degrees Left
    // BABY_ZOMBIE_FRONT_LEFT_IDLE,
    // BABY_ZOMBIE_FRONT_LEFT_STEP_RIGHT,
    // BABY_ZOMBIE_FRONT_LEFT_STEP_LEFT,
    // BABY_ZOMBIE_FRONT_LEFT_STEP_DAMAGED,
    // BABY_ZOMBIE_FRONT_LEFT_STEP_ATTACK,
    //
    // //Baby Zombie Left Side
    // BABY_ZOMBIE_LEFT_IDLE,
    // BABY_ZOMBIE_LEFT_STEP_RIGHT,
    // BABY_ZOMBIE_LEFT_STEP_LEFT,
    // BABY_ZOMBIE_LEFT_DAMAGED,
    // BABY_ZOMBIE_LEFT_ATTACK,
    //
    // //Baby Zombie 45 Degrees Back Right
    // BABY_ZOMBIE_BACK_RIGHT_IDLE,
    // BABY_ZOMBIE_BACK_RIGHT_STEP_RIGHT,
    // BABY_ZOMBIE_BACK_RIGHT_STEP_LEFT,
    // BABY_ZOMBIE_BACK_RIGHT_DAMAGED,
    // BABY_ZOMBIE_BACK_RIGHT_ATTACK,
    //
    // //Baby Zombie Back
    // BABY_ZOMBIE_BACK_IDLE,
    // BABY_ZOMBIE_BACK_STEP_LEFT,
    // BABY_ZOMBIE_BACK_STEP_RIGHT,
    // BABY_ZOMBIE_BACK_DAMAGED,
    // BABY_ZOMBIE_BACK_ATTACK,
    //
    // //Baby Zombie Left
    // BABY_ZOMBIE_BACK_LEFT_IDLE,
    // BABY_ZOMBIE_BACK_LEFT_STEP_LEFT,
    // BABY_ZOMBIE_BACK_LEFT_STEP_RIGHT,
    // BABY_ZOMBIE_BACK_LEFT_DAMAGED,
    // BABY_ZOMBIE_BACK_LEFT_ATTACK,
    //
    // //Baby Zombie Right Side
    // BABY_ZOMBIE_RIGHT_IDLE,
    // BABY_ZOMBIE_RIGHT_STEP_RIGHT,
    // BABY_ZOMBIE_RIGHT_STEP_LEFT,
    // BABY_ZOMBIE_RIGHT_DAMAGED,
    // BABY_ZOMBIE_RIGHT_ATTACK,
    //
    // //Baby Zombie 45 Degrees Right
    // BABY_ZOMBIE_FRONT_RIGHT_IDLE,
    // BABY_ZOMBIE_FRONT_RIGHT_STEP_RIGHT,
    // BABY_ZOMBIE_FRONT_RIGHT_STEP_LEFT,
    // BABY_ZOMBIE_FRONT_RIGHT_STEP_DAMAGED,
    // BABY_ZOMBIE_FRONT_RIGHT_STEP_ATTACK,
    //
    // //Baby Death
    // BABY_ZOMBIE_DEAD,

    //---------- Digger Zombie ----------
    DIGGER_ZOMBIE,
    // DIGGER_ZOMBIE_FRONT_IDLE,
    // DIGGER_ZOMBIE_FRONT_STEP_LEFT,
    // DIGGER_ZOMBIE_FRONT_STEP_RIGHT,
    // DIGGER_ZOMBIE_FRONT_DAMAGED,
    // DIGGER_ZOMBIE_FRONT_ATTACK,
    //
    // //Digger Zombie 45 Degrees Right
    // DIGGER_ZOMBIE_FRONT_LEFT_IDLE,
    // DIGGER_ZOMBIE_FRONT_LEFT_STEP_RIGHT,
    // DIGGER_ZOMBIE_FRONT_LEFT_STEP_LEFT,
    // DIGGER_ZOMBIE_FRONT_LEFT_STEP_DAMAGED,
    // DIGGER_ZOMBIE_FRONT_LEFT_STEP_ATTACK,
    //
    // //Digger Zombie Left Side
    // DIGGER_ZOMBIE_LEFT_IDLE,
    // DIGGER_ZOMBIE_LEFT_STEP_RIGHT,
    // DIGGER_ZOMBIE_LEFT_STEP_LEFT,
    // DIGGER_ZOMBIE_LEFT_DAMAGED,
    // DIGGER_ZOMBIE_LEFT_ATTACK,
    //
    // //Digger Zombie 45 Degrees Back Right
    // DIGGER_ZOMBIE_BACK_RIGHT_IDLE,
    // DIGGER_ZOMBIE_BACK_RIGHT_STEP_RIGHT,
    // DIGGER_ZOMBIE_BACK_RIGHT_STEP_LEFT,
    // DIGGER_ZOMBIE_BACK_RIGHT_DAMAGED,
    // DIGGER_ZOMBIE_BACK_RIGHT_ATTACK,
    //
    // //Digger Zombie Back
    // DIGGER_ZOMBIE_BACK_IDLE,
    // DIGGER_ZOMBIE_BACK_STEP_LEFT,
    // DIGGER_ZOMBIE_BACK_STEP_RIGHT,
    // DIGGER_ZOMBIE_BACK_DAMAGED,
    // DIGGER_ZOMBIE_BACK_ATTACK,
    //
    // //Digger Zombie Left
    // DIGGER_ZOMBIE_BACK_LEFT_IDLE,
    // DIGGER_ZOMBIE_BACK_LEFT_STEP_LEFT,
    // DIGGER_ZOMBIE_BACK_LEFT_STEP_RIGHT,
    // DIGGER_ZOMBIE_BACK_LEFT_DAMAGED,
    // DIGGER_ZOMBIE_BACK_LEFT_ATTACK,
    //
    // //Digger Zombie Right Side
    // DIGGER_ZOMBIE_RIGHT_IDLE,
    // DIGGER_ZOMBIE_RIGHT_STEP_RIGHT,
    // DIGGER_ZOMBIE_RIGHT_STEP_LEFT,
    // DIGGER_ZOMBIE_RIGHT_DAMAGED,
    // DIGGER_ZOMBIE_RIGHT_ATTACK,
    //
    // //Digger Zombie 45 Degrees Right
    // DIGGER_ZOMBIE_FRONT_RIGHT_IDLE,
    // DIGGER_ZOMBIE_FRONT_RIGHT_STEP_RIGHT,
    // DIGGER_ZOMBIE_FRONT_RIGHT_STEP_LEFT,
    // DIGGER_ZOMBIE_FRONT_RIGHT_STEP_DAMAGED,
    // DIGGER_ZOMBIE_FRONT_RIGHT_STEP_ATTACK,
    //
    // //Digger Death
    // DIGGER_ZOMBIE_DEAD,

    //---------- Boss Zombie ----------
    BOSS_ZOMBIE //Sprite sheet, not split
    // BOSS_ZOMBIE_FRONT_IDLE,
    // BOSS_ZOMBIE_FRONT_STEP_LEFT,
    // BOSS_ZOMBIE_FRONT_STEP_RIGHT,
    // BOSS_ZOMBIE_FRONT_DAMAGED,
    // BOSS_ZOMBIE_FRONT_ATTACK,
    //
    // //Boss Zombie 45 Degrees Right
    // BOSS_ZOMBIE_FRONT_LEFT_IDLE,
    // BOSS_ZOMBIE_FRONT_LEFT_STEP_RIGHT,
    // BOSS_ZOMBIE_FRONT_LEFT_STEP_LEFT,
    // BOSS_ZOMBIE_FRONT_LEFT_STEP_DAMAGED,
    // BOSS_ZOMBIE_FRONT_LEFT_STEP_ATTACK,
    //
    // //Boss Zombie Left Side
    // BOSS_ZOMBIE_LEFT_IDLE,
    // BOSS_ZOMBIE_LEFT_STEP_RIGHT,
    // BOSS_ZOMBIE_LEFT_STEP_LEFT,
    // BOSS_ZOMBIE_LEFT_DAMAGED,
    // BOSS_ZOMBIE_LEFT_ATTACK,
    //
    // //Boss Zombie 45 Degrees Back Right
    // BOSS_ZOMBIE_BACK_RIGHT_IDLE,
    // BOSS_ZOMBIE_BACK_RIGHT_STEP_RIGHT,
    // BOSS_ZOMBIE_BACK_RIGHT_STEP_LEFT,
    // BOSS_ZOMBIE_BACK_RIGHT_DAMAGED,
    // BOSS_ZOMBIE_BACK_RIGHT_ATTACK,
    //
    // //Boss Zombie Back
    // BOSS_ZOMBIE_BACK_IDLE,
    // BOSS_ZOMBIE_BACK_STEP_LEFT,
    // BOSS_ZOMBIE_BACK_STEP_RIGHT,
    // BOSS_ZOMBIE_BACK_DAMAGED,
    // BOSS_ZOMBIE_BACK_ATTACK,
    //
    // //Boss Zombie Left
    // BOSS_ZOMBIE_BACK_LEFT_IDLE,
    // BOSS_ZOMBIE_BACK_LEFT_STEP_LEFT,
    // BOSS_ZOMBIE_BACK_LEFT_STEP_RIGHT,
    // BOSS_ZOMBIE_BACK_LEFT_DAMAGED,
    // BOSS_ZOMBIE_BACK_LEFT_ATTACK,
    //
    // //Boss Zombie Right Side
    // BOSS_ZOMBIE_RIGHT_IDLE,
    // BOSS_ZOMBIE_RIGHT_STEP_RIGHT,
    // BOSS_ZOMBIE_RIGHT_STEP_LEFT,
    // BOSS_ZOMBIE_RIGHT_DAMAGED,
    // BOSS_ZOMBIE_RIGHT_ATTACK,
    //
    // //Boss Zombie 45 Degrees Right
    // BOSS_ZOMBIE_FRONT_RIGHT_IDLE,
    // BOSS_ZOMBIE_FRONT_RIGHT_STEP_RIGHT,
    // BOSS_ZOMBIE_FRONT_RIGHT_STEP_LEFT,
    // BOSS_ZOMBIE_FRONT_RIGHT_STEP_DAMAGED,
    // BOSS_ZOMBIE_FRONT_RIGHT_STEP_ATTACK,
    //
    // //Boss Death
    // BOSS_ZOMBIE_DEAD
};

#endif
