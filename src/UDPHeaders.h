/*------------------------------------------------------------------------------
-- SOURCE FILE: UDPHeaders.h - header and struct definition file
--
-- PROGRAM: UDP Headers for MAR-Z
--
-- ENUMS:
--    UDPHeaders
--
-- STRUCTS:
--    Player
--    Action
--    MoveAction
--    AttackAction
--    Zombie
--    GameSync
--
-- DATE: Feb. 07, 2017
--
-- REVISIONS:
-- V1, Feb 07 2017 Deisgned by IM
-- V1, Feb 08 2017 Written by EY
-- V1.5 Feb 25 2017 - EY - chagned to use int32_t type for constistency
--
-- DESIGNER: Isaac Morneau & Eva Yu
--
-- PROGRAMMER: Eva Yu
--
-- NOTES:
-- Header File to define all the structures that contain
-- information to be passed to the server
------------------------------------------------------------------------------*/
#ifndef UDPHEADER_H
#define UDPHEADER_H
#include <stdint.h>
/*------------------------------------------------------------------------------
-- enum: UDPHeaders
--
-- DATE: Feb. 07, 2017
--
-- REVISIONS:
-- V1, Feb 07 2017 Deisgned by IM
-- V1, Feb 08 2017 Written by EY
--
-- DESIGNER: Isaac Morneau
--
-- PROGRAMMER: Eva Yu
--
-- NOTES:
-- this is the enum to hold all the terspecifiers that will indicate the packets
--------------------------------------------------------------------------*/
enum UDPHeaders
{
    //(Main Header indicators in the 0 - 99 domain )
    ATTACK, // 'A'?
    MOVE, // 'M'?
    SYNC, // 'SYN'?
    PLAYERH, // 'P' Player Header
    ZOMBIEH, // 'Z' Zombie Header
    ACTIONH, // 'L' Action Header
    //attack Headers ( in to 100 - 199 DECIMAL domain )
    SHOOT,
    HIT,
    //move headers ( in to 200 - 299 DECIMAL domain )
    WALK,
    //weapon headers ( in to 300 - 399 DECIMAL domain )
    NOWEAPON,
    DEAGLE1,
    RIFLE
};


/*------------------------------------------------------------------------------
-- Struct: AttackAction
--
-- DATE: Feb. 07, 2017
--
-- REVISIONS:
-- V1, Feb 07 2017 Deisgned by IM
-- V1, Feb 08 2017 Written by EY
--
-- DESIGNER: Isaac Morneau
--
-- PROGRAMMER: Eva Yu
--
-- Data Members:
    -- int32_t   id - Attack action specifier
    -- int32_t   actionid -action speicfier
    -- int32_t   weaponid -weapon specifier
    -- float xpos - x Pos
    -- float ypos - y pos
    -- float direction - And Angle in relation to -----
--------------------------------------------------------------------------*/
typedef struct {
	int32_t   id;
	int32_t   actionid;
	int32_t   weaponid;
	float xpos;
	float ypos;
	float direction;
} AttackAction;

/*------------------------------------------------------------------------------
-- Struct: MoveAction
--
-- DATE: Feb. 07, 2017
--
-- REVISIONS:
-- V1, Feb 07 2017 Deisgned by IM
-- V1, Feb 08 2017 Written by EY
--
-- DESIGNER: Isaac Morneau
--
-- PROGRAMMER: Eva Yu
--
-- Data Members:
    -- int32_t   id - int32_t to signify the playerid
    -- float xpos - the X position`
    -- float ypos - the Y position
    -- float xvel - the X velocity`
    -- float yvel - the Y velocity
    -- float direction -- the direction the weapon is facing
--------------------------------------------------------------------------*/
typedef struct
{
	int32_t   id;
	float xpos;
	float ypos;
	float xvel;
	float yvel;
	float direction;
}  MoveAction;

/*------------------------------------------------------------------------------
-- Struct: Action
--
-- DATE: Feb. 07, 2017
--
-- REVISIONS:
-- V1, Feb 07 2017 Deisgned by IM
-- V1, Feb 08 2017 Written by EY
--
-- DESIGNER: Isaac Morneau
--
-- PROGRAMMER: Eva Yu
--
-- Data Members:
    -- int32_t   weaponid - int32_t to signify the playerid
    -- float xpos - the X position`
    -- float ypos - the Y position
    -- float direction -- the direction the weapon is facing
    --
--------------------------------------------------------------------------*/
typedef struct {
	int32_t weaponid;
	float xpos;
	float ypos;
	float direction;
}  Action;

/*------------------------------------------------------------------------------
-- Struct: Player`
--
-- DATE: Feb. 07, 2017
--
-- REVISIONS:
-- V1, Feb 07 2017 Deisgned by IM
-- V1, Feb 08 2017 Written by EY
--
-- DESIGNER: Isaac Morneau
--
-- PROGRAMMER: Eva Yu
--
-- Data Members:
    -- int32_t   playerid - int32_t to signify the playerid
    -- float xpos - the X position`
    -- float ypos - the Y position
    -- float xvel - the x Velocity of the player
    -- float yvel - the y velocity of the player
    -- float direction -- the direction the player is going towards
    -- int32_t   health --
    -- int32_t   actionid --
    -- int32_t   nactions --
    -- Action * actions --
--
--------------------------------------------------------------------------*/
typedef struct {
	int32_t   playerid;
	float xpos;
	float ypos;
	float xvel;
	float yvel;
	float direction;
	int32_t   health;
	int32_t   actionid = ACTIONH;
	int32_t   nactions;
	Action * actions;
} Player;

/*------------------------------------------------------------------------------
-- Struct: Zombie
--
-- DATE: Feb. 07, 2017
--
-- REVISIONS:
-- V1, Feb 07 2017 Deisgned by IM
-- V1, Feb 08 2017 Written by EY
--
-- DESIGNER: Isaac Morneau
--
-- PROGRAMMER: Eva Yu
--
-- Data Members:
    -- int32_t zombieid -- int32_t id of Zombie
    -- int32_t health -- int32_t health of zombie
    -- float xpos -- the x position of zombie`
    -- float ypos -- the y position of zombie
    -- float direction -- the direction the zombie is facing ( angle 0 in respect to 0,0 )
--
--------------------------------------------------------------------------*/

typedef struct {
	int32_t zombieid;
	int32_t health;
	float xpos;
	float ypos;
	float direction;
}  Zombie;

/*------------------------------------------------------------------------------
-- Struct: GameSync
--
-- DATE: Feb. 07, 2017
--
-- REVISIONS:
-- V1, FEbB 07 2017 Created by Eva
--
-- DESIGNER: Isaac Morneau
--
-- PROGRAMMER: Eva Yu
--
-- Data Members:
    --	int32_t id -- int32_t Packet Specifier
    --  int32_t playerheaderid -- int32_t Specifier For Player Header
    --  int32_t nplayers -- int32_t number of Players
    --  Player * -- Point32_ter to the Array of Players
    --  int32_t zombieheaderid -- int32_t Zombie Specifier
    --  int32_t nzombies -- int32_t number of zombies
    --  Zombie * zombies -- point32_ter to the list of zombies
--
--------------------------------------------------------------------------*/
typedef struct
{
	  int32_t id = SYNC; //packet
    int32_t playerheaderid = PLAYERH;
    int32_t nplayers;
    Player * players;
    int32_t zombieheaderid = ZOMBIEH;
    int32_t nzombies;
    Zombie * zombies;
} GameSync;

#endif
