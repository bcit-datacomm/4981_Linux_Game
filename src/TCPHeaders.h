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
struct ConnectPack {
    int  id;
    char username[32];
};

// ReadyPack
struct ReadyPack {
    int  id;
    char ready;
};

// AcceptPack
struct AcceptPack {
    int  id;
    int  playerid;
    char username[32];
};

// PlayerPosition
struct PlayerPosition {
    int playerid;
    int xpos;
    int ypos;
};

// GameStart
struct GameStart {
    int id;
    PlayerPosition * pPositions; // array of player positions
};

#endif
