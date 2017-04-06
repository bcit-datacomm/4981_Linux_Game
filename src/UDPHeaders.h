/*------------------------------------------------------------------------------
* SOURCE FILE: UDPHeaders.h - header and struct definition file
*
* PROGRAM: UDP Headers for MAR-Z
*
* ENUMS:
*    UDPHeaders
*
* STRUCTS:
*    Player
*    Action
*    MoveAction
*    AttackAction
*    Zombie
*    GameSync
*
* DATE: Feb. 07, 2017
*
* REVISIONS:
* V1, Feb 07 2017 Deisgned by IM
* V1, Feb 08 2017 Written by EY
* V1.5 Feb 25 2017 - EY - chagned to use int32_t type for constistency
*
* DESIGNER: Isaac Morneau & Eva Yu
*
* PROGRAMMER: Eva Yu
*
* NOTES:
* Header File to define all the structures that contain
* information to be passed to the server
------------------------------------------------------------------------------*/
#ifndef UDPHEADER_H
#define UDPHEADER_H
#include <cstdint>
/*------------------------------------------------------------------------------
* enum: UDPHeaders
*
* DATE: Feb. 07, 2017
*
* REVISIONS:
* V1, Feb 07 2017 Deisgned by IM
* V1, Feb 08 2017 Written by EY
*
* DESIGNER: Isaac Morneau
*
* PROGRAMMER: Eva Yu
*
* NOTES:
* this is the enum to hold all the terspecifiers that will indicate the packets
--------------------------------------------------------------------------*/
enum class UDPHeaders : int32_t {
    SYNCH, // game sync from server
    PLAYERH, // players
    ZOMBIEH, // zombies
    ATTACKACTIONH, //attacks
    SHOPPURCHASEH, // shop purchase
    BARRICADEACTIONH, // action on barricade
    TURRETACTIONH, // action on turret

    DELETE, //Delete action
    MARINE, //Used for deletion
    ZOMBIE, //Used for deletion
    TURRET, //Used for deletion
    BARRICADE, //Used for deletion
    OBJECT, //Used for deletion
    WEAPON, //Used for deletion
    WEAPONDROP, //Used for deletion

    // movement headers
    WALK,
    DROPOFF,
    PICKUP,
    //attack Headers
    SHOOT,
    HIT,
    //weapon headers
    DEAGLEH,
    RIFLEH,
    //shop item headers
    HEALTHPACKH,
    AMMO,
    TURRETPURCHASE,
    BARRICADEPURCHASE
};

/*------------------------------------------------------------------------------
* Struct: TurretAction
*
* DATE: Mar. 26, 2017
*
* REVISIONS:
* V1, Mar 26 2017 - EY
*
* DESIGNER: Eva Yu
*
* PROGRAMMER: Eva Yu
*
* Data Members:
* int32_t   turretid - the turret being moved
* int32_t   actionid - PICK UP / DROP OFF
* float     xpos - xPosition of the item
* float     ypos - yPosition of the item
--------------------------------------------------------------------------*/
typedef struct {
    int32_t turretid;
    UDPHeaders actionid; // UDPHeaders::PICKUP or UDPHeaders::DROPOFF
    float xpos;
    float ypos;
} __attribute__((packed, aligned(1))) TurretAction;

/*------------------------------------------------------------------------------
* Struct: BarricadeAction
*
* DATE: Mar. 26, 2017
*
* REVISIONS:
* V1, Mar 26 2017 - EY
*
* DESIGNER: Eva Yu
*
* PROGRAMMER: Eva Yu
*
* Data Members:
* int32_t   barricadeid - the barricade being moved
* int32_t   actionid - PICK UP / DROP OFF
* float     xpos - xPosition of the item
* float     ypos - yPosition of the item
*
* NOTE:
* A barricade should not be able to move after being dropped off
--------------------------------------------------------------------------*/
typedef struct {
    int32_t barricadeid;
    UDPHeaders actionid; // UDPHeaders::PICKUP or UDPHeaders::DROPOFF
    float xpos;
    float ypos;
} __attribute__((packed, aligned(1))) BarricadeAction;

/*------------------------------------------------------------------------------
* Struct: ShopPurchase
*
* DATE: Mar. 26, 2017
*
* REVISIONS:
* V1, Mar 26 2017 - EY
*
* DESIGNER: Eva Yu
*
* PROGRAMMER: Eva Yu
*
* Data Members:
* int32_t   shopid - shoip type specifier
* int32_t   itemid - item bought specifier
* int32_t   quantity - number of items bought
* float   cost - total cost of purchase
--------------------------------------------------------------------------*/
typedef struct {
    int32_t shopid;
    int32_t itemid;
    int32_t quantity;
    float cost;
} __attribute__((packed, aligned(1))) ShopPurchase;

/*------------------------------------------------------------------------------
* Struct: AttackAction
*
* DATE: Feb. 07, 2017
*
* REVISIONS:
* V1, Feb 07 2017 Deisgned by IM
* V1, Feb 08 2017 Written by EY
*
* DESIGNER: Isaac Morneau
*
* PROGRAMMER: Eva Yu
*
* Data Members:
* int32_t   id - Attack action specifier
* int32_t   actionid -action speicfier
* int32_t   weaponid -weapon specifier
* float xpos - x Pos
* float ypos - y pos
* float direction - And Angle in relation to -----
--------------------------------------------------------------------------*/
typedef struct {
    int32_t playerid;
    int32_t actionid;
    int32_t weaponid;
    float xpos;
    float ypos;
    float direction;
} __attribute__((packed, aligned(1))) AttackAction;

/*------------------------------------------------------------------------------
* Struct: MoveAction
*
* DATE: Feb. 07, 2017
*
* REVISIONS:
* V1, Feb 07 2017 Deisgned by IM
* V1, Feb 08 2017 Written by EY
* V1.1, Feb 27 2017 - EY - Changed x and y velocity to just velocity to match game logic
* V1.2, Mar 27 2017 - EY - added delta x and y
*
*
* DESIGNER: Isaac Morneau
*
* PROGRAMMER: Eva Yu
*
* Data Members--:
* int32_t   id - int32_t to signify the playerid
* float xpos - the X position`
* float ypos - the Y position
* float xdel - the delta X of the player
* float xdel - the delta Y of the player
* float vel - the velocity of the player
* float direction -- the direction the weapon is facing
--------------------------------------------------------------------------*/
typedef struct {
    int32_t id;
    float xpos;
    float ypos;
    float xdel;
    float ydel;
    float vel;
    float direction;
} __attribute__((packed, aligned(1))) MoveAction;

/*------------------------------------------------------------------------------
* Struct: PlayerData
*
* DATE: Feb. 07, 2017
*
* REVISIONS:
* V1, Feb 07 2017 Deisgned by IM
* V1, Feb 08 2017 Written by EY
* V1, Feb 27 2017 TEMPORARY hardcoding of action structs JA
*
* DESIGNER: Isaac Morneau
*
* PROGRAMMER: Eva Yu
*
* Data Members:
* int32_t   playerid - int32_t to signify the playerid
* float xpos - the X position`
* float ypos - the Y position
* float vel - the Velocity of the player
* float direction - the direction the player is going towards
* int32_t   health - health of the player
--------------------------------------------------------------------------*/

typedef struct {
    int32_t playerid;
    float xpos;
    float ypos;
    float xdel;
    float ydel;
    float vel;
    float direction;
    int32_t health;
} __attribute__((packed, aligned(1))) PlayerData;
/*------------------------------------------------------------------------------
* Struct: ZombieData
*
* DATE: Feb. 07, 2017
*
* REVISIONS:
* V1,  Feb 07 2017 Deisgned by IM
* V1,  Feb 08 2017 Written by EY
* V1.2 Feb 09 2017 Changes by JA
*
* DESIGNER: Isaac Morneau
*
* PROGRAMMER: Eva Yu
*
* Data Members:
* int32_t zombieid -- int32_t id of Zombie
* int32_t health -- int32_t health of zombie
* float xpos -- the x position of zombie`
* float ypos -- the y position of zombie
* float direction -- the direction the zombie is facing ( angle 0 in respect to 0,0 )
*
--------------------------------------------------------------------------*/

typedef struct {
    int32_t zombieid;
    int32_t health;
    float xpos;
    float ypos;
    float direction;
}  __attribute__((packed, aligned(1))) ZombieData;

/*------------------------------------------------------------------------------
* Struct: DeleteAction
*
* DATE: Apr. 03, 2017
*
* DESIGNER: John Agaeyev
*
* PROGRAMMER: John Agapeyev
*
* Data Members:
* UDPHeaders entitytype -- The type of entity that was deleted
* int32_t entityid -- The id of the deleted entity
*
--------------------------------------------------------------------------*/

typedef struct {
    UDPHeaders entitytype;
    int32_t entityid;
}  __attribute__((packed, aligned(1))) DeleteAction;

/*------------------------------------------------------------------------------
* Struct: GameSync
*
* DATE: Feb. 07, 2017
*
* REVISIONS:
* V1, FEbB 07 2017 Created by Eva
*
* DESIGNER: Isaac Morneau
*
* PROGRAMMER: Eva Yu
*
* Data Members:
*    int32_t id -- int32_t Packet Specifier
*  int32_t playerheaderid -- int32_t Specifier For Player Header
*  int32_t nplayers -- int32_t number of Players
*  Player * -- Point32_ter to the Array of Players
*  int32_t zombieheaderid -- int32_t Zombie Specifier
*  int32_t nzombies -- int32_t number of zombies
*  Zombie * zombies -- point32_ter to the list of zombies
*
--------------------------------------------------------------------------*/
typedef struct
{
    int32_t id = static_cast<int32_t>(UDPHeaders::SYNCH); //packet
    int32_t playerheaderid = static_cast<int32_t>(UDPHeaders::PLAYERH);
    int32_t nplayers;
    PlayerData *players;
    int32_t attackheaderid = static_cast<int32_t>(UDPHeaders::ATTACKACTIONH);
    int32_t nattacks;
    AttackAction *attacks;
    int32_t zombieheaderid = static_cast<int32_t>(UDPHeaders::ZOMBIEH);
    int32_t nzombies;
    ZombieData *zombies;
    int32_t deleteheaderid = static_cast<int32_t>(UDPHeaders::DELETE);
    int32_t ndeletes;
    DeleteAction *deletions;
} GameSync;

/*------------------------------------------------------------------------------
* UNION: PacketData
*
* DATE: Mar. 15, 2017
*
* * REVISIONS:
* V1 - Mar 15 2017 Created by JA
* V1.5 - Mar 15 2017 comments and additions by EY
*
* DESIGNER: John Agapeyev
*
* PROGRAMMER: John Agapeyev
*
* Data Members:
*    MoveAction
*   AttackAction
*   ShopPurchase
*   TurretAction
*   BarricadeAction
--------------------------------------------------------------------------*/
union PacketData {
    MoveAction ma;
    AttackAction aa;
    ShopPurchase sp;
    TurretAction ta;
    BarricadeAction ba;
    DeleteAction da;
};

/*------------------------------------------------------------------------------
* STRUCT: ClientMessage
*
* DATE: Mar. 15, 2017
*
* * REVISIONS:
* V1 - Mar 15 2017 Created by JA
* V1.5 - Mar 15 2017 comments and additions by EY
*
* DESIGNER: John Agapeyev
*
* PROGRAMMER: John Agapeyev
*
* Data Members:
*    int32_t id - the id of the packet type to the server
*    PacketData - union of the packet to send to server
--------------------------------------------------------------------------*/
struct ClientMessage {
    int32_t id;
    PacketData data;
};

#endif
