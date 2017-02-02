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

//moved enums to the bottom, and commented out

class Client
{
public:
    /*--------------------------------------------------------------------------
    CONSTRUCTOR: Client

    DATE: Febrauary. 01, 2017

    REVISIONS:
    Version, Date and Description

    DESIGNER: Brody McCrone

    PROGRAMMER: Brody McCrone

    INTERFACE: {5:returntype} {6:fucntionName} (NetworkQueue * in,
        NetworkQueue *out, const char *ip, const *username)
    in: Queue for incoming TCP data.
    out: Queue for outgoing TCP data.

    NOTES:
    Creates a thread in which this classes' run method runs. Check the in
    queue for incoming data and write data you wish to send to the out queue.
    --------------------------------------------------------------------------*/
	Client(NetworkQueue *, NetworkQueue *, const char *, const char *);
	/*--------------------------------------------------------------------------
	METHOD: run

	DATE: February. 1, 2017

	REVISIONS:
	Version, Date and Description

	DESIGNER: Brody McCrone

	PROGRAMMER: Brody McCrone

	INTERFACE:  void run (const char *ip, const char *username)
	ip: IP of the server.
	username: Client's username.

	RETURNS:
	void

	NOTES:
    Performs the first part of the client-server handshake. i.e Sends the server
	the client's username, receives the integer id assigned to the client, and
	then received the integer ids and usernames of other clients connecting to
	the server.
	--------------------------------------------------------------------------*/
	void run(const char *, const char *);

	//unused public methods
	//void end();
	//void handleError();
	//void cleanup();
private:
    int _sockTCP;
    NetworkQueue *_in;
    NetworkQueue *_out;
    std::atomic<bool> _running;


    int TCPConnect(const char *);
    int writeTCPSocket(const char *, int);
    /*------------------------------------------------------------------------------
    METHOD: readTCPSocket

    DATE: February. 1, 2017

    REVISIONS:
    Version, Date and Description

    DESIGNER: Brody McCrone

    PROGRAMMER: Brody McCrone

    INTERFACE: int readTCPSocket (char * buf, int len)
    buf: Buffer to store read result in.
    len: Amount of characters to read, can be no more than buffer size.

    RETURNS:
    -1: error (check errno for further info)
    0: connection was closed.
    len: read was successful.

    NOTES:
    This read method reads the amount specified by the param len from the TCP
    socket stored as a private member of the Client object.
    --------------------------------------------------------------------------*/
	int readTCPSocket(char *, int);

	//unused private methods
	//int readTCPSocket(char *, int, int);
	//const char * packetize(const int, const  char *);
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
