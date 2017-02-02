#include <stdio.h>
#include <iostream>
#include <string.h>
#include <string>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <thread>
#include <errno.h>
#include <sys/epoll.h>
#include "Client.h"

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
Client::Client(NetworkQueue *in, NetworkQueue *out,
	const char *ip, const char *username)
:_sockTCP(0), _in(in), _out(out), _running(true) {
	std::thread netThread(&Client::run, this, ip, username);
	netThread.detach();
}

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
void Client::run(const char *ip, const char *username) {
	TCPConnect(ip);

	char users[MAX_USERS][UNAME_BUFFSIZE];
	char sendline[UNAME_BUFFSIZE];
	char recvline[UNAME_BUFFSIZE + 1];

	strcpy(sendline, username);
	memset(sendline + strlen(username), 0, UNAME_BUFFSIZE);

	writeTCPSocket(sendline, UNAME_BUFFSIZE);

	while(readTCPSocket(recvline, UNAME_BUFFSIZE + 1) != 0) {
		strncpy(users[(int)recvline[UNAME_BUFFSIZE]], recvline, UNAME_BUFFSIZE);
		_in->write(users[(int)recvline[UNAME_BUFFSIZE]]);
	}

	close(_sockTCP);
	std::cout << "Connection closed." << std::endl;
}

int Client::writeTCPSocket(const char * msg, int len) {
	int res = 0;
	int ttlsent = 0, bytesleft = len;
	while (ttlsent < len) {
			if ((res = send(_sockTCP, msg + ttlsent, bytesleft, 0)) < 0 ) {
				perror("TCP write error");
				return res;
			}
			ttlsent += res;
			bytesleft -= res;
	}
	return 0;
}

/*------------------------------------------------------------------------------
METHOD: readTCPSocket

DATE: February. 1, 2017

REVISIONS:
Version, Date and Description

DESIGNER: Eva Yu

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
int Client::readTCPSocket(char *buf, int len) {
	int res = 0;
	int ttlsent = 0, bytesleft = len;
	while (ttlsent < len) {
			if ((res = read(_sockTCP, buf + ttlsent, bytesleft)) < 0 ) {
				perror("read");
				return res;
			}
			ttlsent += res;
			bytesleft -= res;
	}
	return res;
}

int Client::TCPConnect(const char * ip_addr) {
	if (ip_addr == NULL || inet_addr(ip_addr) == 0 ) {
		std::cerr << "Missing or Incorrect IP addr."
			<< "\n IP Address must be in the form x.x.x.x"
			<< std::endl;
		return -1;
	}

	struct sockaddr_in serv_addr;

	memset(&serv_addr, '0', sizeof(struct sockaddr_in));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(TCP_PORT);
	serv_addr.sin_addr.s_addr = inet_addr(ip_addr);

	if ((_sockTCP = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		std::cerr << "Error opening socket" << std::endl;
		exit(1);
	}

	if((connect(_sockTCP, (struct sockaddr *)&serv_addr,
        sizeof(struct sockaddr_in))) < 0) {
		perror("Error Connecting");
		exit(1);
	}

	return 0;
}

/*
//Currently unused mechanism for killing network thread
void Client::end() {
	_running = false;
}


//Currently unused and convoluted use of epoll to make tcpfd read nonblocking.
int Client::readTCPSocket(char *buf, int bufSize, int epfd)
{
	//CODE TO SET EPOLL ON TCP_SOCKFD READ - WULD NOT BE EXECUTED HERE
	int epfd;
	if((epfd = epoll_create(1)) < 0)
        error("epoll_create");

	struct epoll_event event;
	int ret;

	event.data.fd = tcp_sockfd;
	event.events = EPOLLIN;

	if(epoll_ctl (epfd, EPOLL_CTL_ADD, fd, &event)) < 0)
        error("epoll_ctl");
  //END OF CODE TO SET EPOLL

	struct epoll_event events[MAX_EVENTS];
	int numEvents;
	int numRead;
	if((numEvents = epoll_wait(epfd, events, MAX_EVENTS, 0) < 0))
		perror("epoll_wait");

	for(int i = 0; i < numEvents; i++) {
        if(events[i].data.fd == _tcpsockfd) {
			if(events[i].data.fd & EPOLLIN) {
				if((numRead = read(_tcpsockfd, buf, bufSize)) < 0)
					perror("read");
				return numRead;
			}
		}
	}

	return -1;
}

//unused
const char * Client::packetize(const int specifier, const char * msg)
{
	char * cbuff;
	int buffsize;

	//determine buff size
	switch(static_cast<P_SPECIFIER>(specifier))
	{
		case P_SPECIFIER::EXIT:
				//cleanup();
				exit(0);
				break;
		case P_SPECIFIER::UNAME:
			buffsize = UNAME_BUFFSIZE;
			break;
		case P_SPECIFIER::CHAT:
			buffsize = CHAT_BUFFSIZE;
		break;
		default: // error encountered.
			std::cerr << "Error occured. Undecalred Specifier." << std::endl;
			return NULL;
	}

    //creating the packet
	cbuff = (char * )malloc(buffsize+1);
	//is message too long? Cut it short.
	if (static_cast<int>(strlen(msg)) >= buffsize)
	{
		memcpy(cbuff, msg, buffsize);
		cbuff[buffsize-1]= '\0';
	}
	else
	{
		memcpy(cbuff, msg, strlen(msg));
		memset(cbuff + strlen(msg),'\0', UNAME_BUFFSIZE-strlen(msg));
	}

	//writeTCPSocket(buff);
    //REMINDER : REMEMBER TO FREE CBUFF AFTER YOU ARE DONE WITH IT!!!
	//free(cbuff);

	return cbuff;
}

//old write that doesn't handle MTU issues.
int Client::writeTCPSocket(char *buf, int len)
{
	int nWrote;
	if((nWrote = write(_tcpsockfd, buf, len)) < 0)
	{
		perror("write");
		return -1;
	}

	return nWrote;
}
*/
