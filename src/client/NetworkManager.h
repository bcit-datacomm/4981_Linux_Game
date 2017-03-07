#ifndef NETWORKMANAGER_HPP
#define NETWORKMANAGER_HPP

#include "UDPSocket.h"
#include <stdio.h>
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <limits.h>
#include <atomic>
#include <memory>

#define TCP_PORT 		35223
#define UDP_PORT 		35222
#define UNAME_SIZE 	    32
#define PLAYERPACK_SIZE (UNAME_SIZE + 1)
#define CHAT_BUFFSIZE 	128
#define STD_BUFFSIZE    1024
#define MAX_EVENTS 		1
#define MAX_USERS		23
#define SYNC_PACKET_MAX USHRT_MAX
#define SYNC_PACKET


//moved enums to the bottom, and commented out

class NetworkManager {
public:
    static NetworkManager& instance();

    UDPSocket& getSockUDP() {return _sockUDP;};
    void handshake(const char *, const char *, int);

private:
    int _sockTCP;
    UDPSocket _sockUDP;

    NetworkManager() {};

    void runUDPClient(std::shared_ptr<UDPSocket> udpSock);
    int TCPConnect(const char *);
    int writeTCPSocket(const char *, int);
	int readTCPSocket(char *, int);
};

#endif

/*
//ENUMS
#ifndef PACKET_SPECIFIER_ENUMCL
#define PACKET_SPECIFIER_ENUMCL

enum class P_SPECIFIER{
	  EXIT,
		UNAME,
		CHAT
};

#endif

#ifndef GAMESTATE_ENUMCL
#define GAMESTATE_ENUMCL

enum class GAMESTATE{
		GAME_RECV,
		AI_RECV
};
#endif
*/
