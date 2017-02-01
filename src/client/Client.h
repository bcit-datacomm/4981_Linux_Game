#ifndef CLIENT_HPP
#define CLIENT_HPP

#define TCP_PORT 		35223
#define UDP_PORT 		35222
#define UNAME_BUFFSIZE 	32
#define CHAT_BUFFSIZE 	128
#define MAX_EVENTS 		1	
#define MAX_USERS		23

#include <stdio.h>
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <atomic>
#include "NetworkQueue.hpp"


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

class Client
{
public:
	Client(NetworkQueue *, NetworkQueue *, const char *, const char *);
	//~Client();
	void run(const char *, const char *);
	void end();
	
	//void handleError();
	//void cleanup();
private:
	int _tcpsockfd;
	NetworkQueue *_in;
	NetworkQueue *_out;
	std::atomic<bool> _running;

	int TCPConnect(const char *);
	int writeTCPSocket(const char *, int);
	int readTCPSocket(char *, int);
	
	//unused
	int readTCPSocket(char *, int, int);
	const char * packetize(const int, const  char *);
};
#endif
