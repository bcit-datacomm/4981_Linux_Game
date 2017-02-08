#ifndef TCPPACKETS_H
#define TCPPACKETS_H

// TCP Header Specifiers
enum TCP_ID {
    USERNAME, // 'n'?
    PLAYERID, // 'i'?
    ACCEPTED, // 's'?
    GAMESTART, //'??' char?
    READY = 0x72, //char = 'r'
    UNREADY = 0x75 //char = 'u'
};

// ConnectPack
typedef struct {
    int  id;
    char username[32];
} ConnectPack;

// ReadyPack
typedef struct {
    int  id;
    char ready;
} ReadyPack;

// AcceptPack
typedef struct {
    int  id;
    int  playerid;
    char username[32];
} AcceptPack;

// PlayerPosition
typedef struct {
    int playerid;
    int xpos;
    int ypos;
} PlayerPosition;

// GameStart
typedef struct {
    int id;
    PlayerPosition * pPositions; // array of player positions
} GameStart;

#endif
