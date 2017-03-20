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
#include "packetizer.h"

using namespace std;

bool networked = false;


/**------------------------------------------------------------------------------
-------------------------------------------------------------------------------*/
NetworkManager& NetworkManager::instance() {
    static NetworkManager sInstance;
    return sInstance;
}

/**------------------------------------------------------------------------------
-------------------------------------------------------------------------------*/
NetworkManager::~NetworkManager() {
    close(sockTCP);
    close(sockUDP);
}

/**------------------------------------------------------------------------------
METHOD: run

DATE: February. 1, 2017

DESIGNER: Brody McCrone

PROGRAMMER: Brody McCrone

INTERFACE: void run(std::string ip, std::string username)
ip: IP of the server.
username: Client's username.

RETURNS:
void

NOTES:
Creates TCP, and UDP sockets, bind sockets, connects TCP socket, and starts threads
for TCP Client and UDP Client.
-------------------------------------------------------------------------------*/
void NetworkManager::run(const std::string ip, const std::string username) {
    state = NetworkState::INITIALIZING;

    in_addr_t serverIP = inet_addr(ip.c_str());
    sockTCP = createSocket(SOCK_STREAM);
    bindSock(sockTCP, createAddress(INADDR_ANY, TCP_PORT));
    connectSocket(sockTCP, createAddress(serverIP, TCP_PORT));
    servUDPAddr = createAddress(serverIP, UDP_PORT);
    servUDPAddrLen = sizeof(servUDPAddr);
    sockUDP = createSocket(SOCK_DGRAM);
    bindSock(sockUDP, createAddress(INADDR_ANY, UDP_PORT));

    std::thread TCPThread(&NetworkManager::runTCPClient, this, username);
    TCPThread.detach();

    std::thread UDPThread(&NetworkManager::runUDPClient, this);
    UDPThread.detach();
}

/**------------------------------------------------------------------------------
-------------------------------------------------------------------------------*/
void NetworkManager::runUDPClient() {
    char buffer[SYNC_PACKET_MAX];
    int packetSize;
    packetSize = readUDPSocket(buffer, SYNC_PACKET_MAX);
    state = NetworkState::GAME_STARTED;
    Packetizer::parseGameSync(buffer, packetSize);

    for(;;) {
        packetSize = readUDPSocket(buffer, SYNC_PACKET_MAX);
        Packetizer::parseGameSync(buffer, packetSize);
    }
}

/**------------------------------------------------------------------------------
-------------------------------------------------------------------------------*/
void NetworkManager::runTCPClient(const std::string username) {
    std::cout << "username in runTCPClient: " << username << std::endl;
    handshake(username);
    waitRecvId();

    /***************************************************/
    /*FOLLOWING TO BE REMOVED WHEN GAME IS INTEGRATED*/
    fd_set initSet;
    fd_set readSet;
    char buffrecv[STD_BUFFSIZE];
    char buffsend[STD_BUFFSIZE];
    int bytesRead;

    FD_ZERO(&initSet);
    FD_SET(STDIN, &initSet);
    FD_SET(sockTCP, &initSet);

    for(;;) {
        readSet = initSet;

        if (select(sockTCP + 1, &readSet, NULL, NULL, NULL) < 0) {
            perror("select");
            exit(1);
        }

        if (FD_ISSET(STDIN, &readSet)) {
            int bytesToSend;
            string str;
            getline(cin, str);
            bytesToSend = Packetizer::packControlMsg(buffsend, STD_BUFFSIZE, str.c_str(), myid);
            writeTCPSocket(buffsend, bytesToSend);
        }

        if (FD_ISSET(sockTCP, &readSet)) {
            if ((bytesRead = readTCPSocket(buffrecv, STD_BUFFSIZE)) == 0) {
                std::cout << "TCP Connection closed by server. (Bytes read == 0)" << std::endl;
                break;
            }

            Packetizer::parseControlMsg(buffrecv, bytesRead);
        }
    }
    /***************************************************/

    close(sockTCP);
}

/**------------------------------------------------------------------------------
-------------------------------------------------------------------------------*/

void NetworkManager::handshake(const std::string username) const {
    char sendline[STD_BUFFSIZE] = {0};
    int bytesToSend;

    //packetize the username first
    bytesToSend = Packetizer::packControlMsg(sendline, STD_BUFFSIZE, username.c_str());
    writeTCPSocket(sendline, bytesToSend); //send the username to server.
}

/**------------------------------------------------------------------------------
-------------------------------------------------------------------------------*/

void NetworkManager::waitRecvId() {
    char buffrecv[STD_BUFFSIZE];
    int bytesRead;
    bytesRead = readTCPSocket(buffrecv, STD_BUFFSIZE);
    Packetizer::parseControlMsg(buffrecv, bytesRead);
    // !!!!replace with parseControlMsg
    myid = *(reinterpret_cast<const int32_t *>(buffrecv));
}

/**------------------------------------------------------------------------------
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
-------------------------------------------------------------------------------*/
void NetworkManager::writeTCPSocket(const char *buf, int len) const {
    int res = send(sockTCP, buf, len, 0);
    if (res < 0) {
        perror("send");
        exit(1);
    }

    /* this assertion is here because there were concerns that our send would exceed an MTU and
    wouldn't fully send. */
    assert(res == len);
}

/**------------------------------------------------------------------------------
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
-------------------------------------------------------------------------------*/
int NetworkManager::readTCPSocket(char *buf, const int len) const {
    int res = 0;
    if ((res = read(sockTCP, buf, len)) < 0) {
        perror("read");
        return res;
    }

    return res;
}

/**------------------------------------------------------------------------------
_servAddr---
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
-------------------------------------------------------------------------------*/
void NetworkManager::writeUDPSocket(const char *buf, const int &len) const {
    /* NetworkManager::run should be called before this is writeUDPSocket is called. */
    assert(state != NetworkState::NOT_RUNNING);

    int res = sendto(sockUDP, buf, len, 0, (const sockaddr *)&servUDPAddr, servUDPAddrLen);
    if (res < 0) {
		perror("read");
        exit(1);
	}

    /* this assertion is here because there were concerns that our send would exceed an MTU and
    wouldn't fully send. */
    assert(res == len);
}


/**------------------------------------------------------------------------------
METHOD: readUDPSocket

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
-------------------------------------------------------------------------------*/
int NetworkManager::readUDPSocket(char *buf, const int& len) const {
    sockaddr_in recvAddr;
    socklen_t recvAddrLen = sizeof(recvAddr);
    memset(&recvAddr, 0, recvAddrLen);
    int res = recvfrom(sockUDP, buf, len, 0, (struct sockaddr *)&recvAddr, &recvAddrLen);
    if (res < 0) {
		perror("recvfrom");
        exit(1);
	}

    return res;
}

/**------------------------------------------------------------------------------
-------------------------------------------------------------------------------*/
void NetworkManager::connectSocket(int sock, const sockaddr_in& addr) {
    if ((connect(sock, (struct sockaddr *)&addr, sizeof(sockaddr_in))) < 0) {
        perror("connect");
        exit(1);
    }
}

/**------------------------------------------------------------------------------
-------------------------------------------------------------------------------*/
sockaddr_in NetworkManager::createAddress(const in_addr_t ip, const int port) {
    sockaddr_in addr;
    //set server addr struct
    memset(&addr, '0', sizeof(sockaddr_in));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = ip;

    return addr;
}

/**------------------------------------------------------------------------------
-------------------------------------------------------------------------------*/
int NetworkManager::createSocket(int sockType) {
    int sock = socket(AF_INET, sockType, 0);
    if (sock < 0) {
        perror("sock");
        exit(1);
    }

    return sock;
}

/**------------------------------------------------------------------------------
-------------------------------------------------------------------------------*/
void NetworkManager::bindSock(int sock, sockaddr_in addr) {
    int optionValue = 1;
    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &optionValue, sizeof(optionValue))) {
        perror("sockopt");
        exit(1);
    }

    if (bind(sock, (sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("bind");
        exit(1);
    }
}
