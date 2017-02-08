#ifndef UDPHEADER_H
#define UDPHEADER_H

enum UDPHeaders
{
    //(Main Header indicators in the 0 - 99 domain )
    ATTACK, // 'A'?
    MOVE, // 'M'?
    SYNC, // 'SYN'?
    PLAYERRECS, // 'P' Player Header
    ZOMBIERECS, // 'Z' Zombie Header
    ACTIONRECS, // 'L' Action Header
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

// attackAction
typedef struct {
	int   id;
	int   actionid;
	int   weaponid;
	float xpos;
	float ypos;
	float direction;
} AttackAction;

//MoveAction
typedef struct
{
	int   id;
	float xpos;
	float ypos;
	float xvel;
	float yvel;
	float direction;
}  MoveAction;

//ActionRecords
typedef struct {
	int weaponid;
	float xpos;
	float ypos;
	float direction;
}  Action;

typedef struct {
	int   playerid;
	float xpos;
	float ypos;
	float xvel;
	float yvel;
	float direction;
	int   health;
	int   actionid;
	int   nactions;
	Action * actions;
} Player;

typedef struct {
	int zombieid;
	int health;
	int xpos;
	int ypos;
	float direction;
}  Zombie;

typedef struct
{
	  int id; //packet
    int playerheaderid;
    int nplayers;
    Player * players;
    int zombieheaderid;
    int nzombies;
    Zombie * zombies;
} GameSync;

#endif
