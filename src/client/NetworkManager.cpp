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
#include <string>
#include <iterator>

#include "NetworkManager.h"
#include "../packetizer.h"

using namespace std;

NetworkManager& NetworkManager::instance() {
    static NetworkManager sInstance;
    return sInstance;
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

void NetworkManager::initClients(const char *ip) {
    serverIP = inet_addr(ip);

    std::thread UDPThread(&NetworkManager::runUDPClient, this);
    UDPThread.detach();

    std::thread TCPThread(&NetworkManager::runTCPClient, this);
    TCPThread.detach();
}

void NetworkManager::run(const char *ip, const char  * username)
{
    sockTCP = createSocket(SOCK_STREAM);
    bindSocket(sockTCP, createAddress(INADDR_ANY, TCP_PORT));
    connectSocket(sockTCP, createAddress(serverIP, TCP_PORT));
    connected = true;

    handshake(ip,username); //handshake
    waitRecvId();

    std::thread UDPThread(&NetworkManager::runUDPClient, this);

    /***************************************************/
    /*FOLLOWING TO BE REMOVED WHEN GAME IS INTEGRATED:*/
    fd_set readSet, initSet;
    char buffrecv[STD_BUFFSIZE], buffsend[STD_BUFFSIZE];
    int bRead;

    FD_ZERO(&initSet);
    FD_SET(STDIN, &initSet);
    FD_SET(sockTCP, &initSet);

    do {
        readSet = initSet;

        if(select(sockTCP + 1, &readSet, NULL, NULL, NULL) < 0) {
            perror("select");
            exit(1);
        }

        if(FD_ISSET(STDIN, &readSet)) {
            int bytesToSend;
            string str;
            getline(cin, str);
            bytesToSend = Packetizer::packControlMsg(buffsend, STD_BUFFSIZE, str.c_str(), _myid);
            writeSocket(sockTCP, buffsend, bytesToSend);
        }

        if(FD_ISSET(sockTCP, &readSet)) {
            if ((bRead = readSocket(sockTCP, buffrecv, STD_BUFFSIZE)) == 0 ) {
                perror("Connection Closed");
                connected = false;
                break;
            }

            if(!running){
                cout << "\nNew player detected! username: " << (buffrecv + 6);
            }else{ // if game is going, must be player message
                cout << "\nChat Recieved! msg: " << (buffrecv + 6);
            }
        }
    } while(connected);
    UDPThread.join();
    /***************************************************/
}

void NetworkManager::handshake(const char *ip, const char *username) {

    char sendline[STD_BUFFSIZE] = {0};
    int bytesToSend;
    //packetize the username first
    bytesToSend = Packetizer::packControlMsg(sendline, STD_BUFFSIZE, username);
    writeSocket(sockTCP, sendline, bytesToSend); //send the username to server.
}

void NetworkManager::waitRecvId()
{
    char buffrecv[STD_BUFFSIZE];
    readSocket(sockTCP, buffrecv, STD_BUFFSIZE);
    // !!!!replace with parseControlMsg
        _myid = *(reinterpret_cast<const int32_t *>(buffrecv));
}

void NetworkManager::runTCPClient() {
    fd_set readSet, initSet;
    char buffer[USHRT_MAX];
    int bytesRead;

    sockTCP = createSocket(SOCK_STREAM);
    bindSocket(sockTCP, createAddress(INADDR_ANY, TCP_PORT));
    connectSocket(sockTCP, createAddress(serverIP, TCP_PORT));

    FD_ZERO(&initSet);
    FD_SET(STDIN, &initSet);
    FD_SET(sockTCP, &initSet);
    for(;;) {
        readSet = initSet;
        if(select(sockTCP + 1, &readSet, NULL, NULL, NULL) < 0) {
            perror("select");
            exit(1);
        }

        if(FD_ISSET(STDIN, &readSet)) {
            //get command
            fgets(buffer, USHRT_MAX, stdin);
            //replace new line with null
            buffer[strlen(buffer) - 1] = '\0';
            //send command
            writeSocket(sockTCP, buffer, strlen(buffer));
        }

        if(FD_ISSET(sockTCP, &readSet)) {
            if((bytesRead = readSocket(sockTCP, buffer, USHRT_MAX)) == 0) {
                std::cout << "Server disconnected." << std::endl;
                exit(1);
            }
            Packetizer::parseControlMsg(buffer, bytesRead);
        }
    }
}

void NetworkManager::closeConnection()
{
    close(sockTCP);
}

void NetworkManager::runUDPClient() {
    sockUDP = createSocket(SOCK_DGRAM);
    bindSocket(sockUDP, createAddress(INADDR_ANY, UDP_PORT));
    connectSocket(sockUDP, createAddress(serverIP, UDP_PORT));

    char buffer[SYNC_PACKET_MAX];
    int packetSize;

    packetSize = readSocket(sockUDP, buffer, SYNC_PACKET_MAX);
    cout << "\nRecevied Dgram. Bytes read: " << packetSize;
    running = true;
    //Packetizer packetizer;
    while(running) {
        packetSize = readSocket(sockUDP, buffer, SYNC_PACKET_MAX);
        writeSocket(sockUDP, buffer, packetSize);
        cout << "\nRecevied Dgram. Bytes read: " << packetSize;
        /*TEMP:*/
        //Packetizer::parseGameSync(buffer, packetSize);
    }
}

/*--------------------------------------------------------------------------
-- FUNCTION: writeTCPSocket
--
-- DATE: FEB. 01, 2017
--
-- REVISIONS:
-- Version 1.0 - [EY] - 2016/FEB/01 - Created Function
--
-- DESIGNER: Brody Mccrone
--
-- PROGRAMMER: Eva Yu
--
-- INTERFACE: int writeTCPSocket(const char * msg, int len)
-- const char * -- the message to write to the TCP socket
-- int len -- the length of the message to write
--
-- RETURNS:
--  0 represents excution success
-- -1 represents failure
--
-- NOTES:
-- This function loops the writing to the TCP Socket
--------------------------------------------------------------------------*/
int NetworkManager::writeSocket(int sock, const char *msg, int len) {
    int res = 0;
    int ttlsent = 0, bytesleft = len;
    while (ttlsent < len) {
        if ((res = send(sockTCP, msg + ttlsent, bytesleft, 0)) < 0 ) {
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
int NetworkManager::readSocket(int sock, char *buf, int len) const {
    int res = 0;
    if ( (res = read(sock, buf, len) ) < 0 ) {
        perror("read");
        return res;
    }
    return res;
}

/*--------------------------------------------------------------------------
-- FUNCTION: TCPConnect
--
-- DATE: FEB. 01, 2017
--
-- REVISIONS:
-- Version 1.0 - [EY] - 2016/FEB/01 - Created Function
--
-- DESIGNER: Eva Yu
--
-- PROGRAMMER: Eva Yu
--
-- INTERFACE: int TCPConnect (const char *)
-- const char * The ip_address
--
-- RETURNS:
-- int representing state : -1 is failure ; 0 is success
--
-- NOTES:
-- This is afunction that connects the user with via TCP
--------------------------------------------------------------------------*/
void NetworkManager::connectSocket(int sock, const struct sockaddr_in& addr) const {
    if((connect(sock, (struct sockaddr *)&addr, sizeof(struct sockaddr_in))) < 0) {
        perror("connect");
        exit(1);
    }
}

struct sockaddr_in NetworkManager::createAddress(const in_addr_t ip, const int port) const {
    struct sockaddr_in addr;
    //set server addr struct
    memset(&addr, '0', sizeof(struct sockaddr_in));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = ip;

    return addr;
}

int NetworkManager::createSocket(int sockType) const {
    int sock = socket(AF_INET, sockType, 0);
    if(sock < 0) {
        perror("sock");
        exit(1);
    }
    return sock;
}

void NetworkManager::bindSocket(int sock, struct sockaddr_in addr) {
    if(bind(sock, (sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("bind");
        exit(1);
    }
}

void NetworkManager::writeUDPSocket(const char *buf, int len) {
    writeSocket(sockUDP, buf, len);
}

/*-----------------------------------------------------------------------_servAddr---
METHOD: sendTo

DATE: Feb. 7, 2017

REVISIONS:
Version, Date and Description

DESIGNER: Brody McCrone

PROGRAMMER: Brody McCrone

PARAMS:
buf: Data to be sent.
len: Amount of data. Must be equal to or less than buf size.
servAddr: Address of receiver.

RETURNS:
void

NOTES:
Sends buf to servAddr. Reliable even when len exceeds MTU.
--------------------------------------------------------------------------*/
/*
void NetworkManager::writeUDPSocket(const char *buf, const int &len) {
    socklen_t addrLen = sizeof(_servAddr);
    int res = 0;
	int ttlsent = 0, bytesleft = len;
	while (ttlsent < len) {
        if ((res = sendto(sockUDP, buf + ttlsent, bytesleft, 0,
            (struct sockaddr *)&_servAddr, addrLen)) < 0 ) {
			perror("read");
            exit(1);
		}
		ttlsent += res;
		bytesleft -= res;
	}
}
*/

/*--------------------------------------------------------------------------
METHOD: receiveFrom

DATE: Feb. 7, 2017

REVISIONS:
Version, Date and Description

DESIGNER: Brody McCrone

PROGRAMMER: Brody McCrone

PARAMS:
buf: Buffer to store data received in.
len: Amount of data to receive. Must be less than or equal to buf size.
servAddr: Address of sender.

RETURNS:
void

NOTES:
Receives data from servAddr and stores it in buf. Reliable even when data
being received exceeds MTU.
--------------------------------------------------------------------------*/
/*
int NetworkManager::readUDPSocket(char *buf, const int& len) {
    socklen_t addrLen = sizeof(_servAddr);
    int res = recvfrom(sockUDP, buf, len, 0,
        (struct sockaddr *)&_servAddr, &addrLen);
    if(res < 0 ) {
		perror("recvfrom");
        exit(1);
	}

    return res;
}
*/
