
#ifndef CLIENT_HPP
#define CLIENT_HPP

#define TCP_PORT 				35222
#define UDP_PORT 	   		35223
#define UNAME_BUFFSIZE 	32
#define CHAT_BUFFSIZE 	128

#include <stdio.h>
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>


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
	Client();
	~Client();
	void handleError();
	int TCPConnect(const char * );
	void cleanup();
private:
	int tcp_sockfd;
	char * username;
	char * chat_buffer;
	//struct sockaddr_in * serv_addr;

	void initClient();
	void readFromOutputQueue();
	const char * packetize(const int, const  char *);
	void parseData(char *);
	void writeTCPSocket();
	void readTCPSocket();
};
#endif
