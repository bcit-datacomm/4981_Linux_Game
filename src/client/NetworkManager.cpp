#include <cstdio>
#include <iostream>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <thread>
#include <string>
#include <cassert>

#include "../server/serverwrappers.h"
#include "NetworkManager.h"
#include "packetizer.h"
#include "../log/log.h"

using namespace std;

bool networked = false;

/**------------------------------------------------------------------------------
Method: instance

Date: February. 1, 2017

Designer: Brody McCrone

Programmer: Brody McCrone

Interface: NetworkManager& instance()

Returns:
NetworkManager&: Introducing the one and only NetworkManager!

Notes:
Creates NetworkManager the first time it is called, and returns that NetworkManager.
Subsequent calls to instance return the NetworkManager created in the first call.
  -------------------------------------------------------------------------------*/
NetworkManager& NetworkManager::instance() {
    static NetworkManager sInstance;
    return sInstance;
}

/**------------------------------------------------------------------------------
Method: ~NetworkManager

Date: February. 1, 2017

Designer: Eva Yu

Programmer: Eva Yu

Interface: NetworkManager::~NetworkManager()

Notes:W
Closes all NetworkManager sockets.
  -------------------------------------------------------------------------------*/
NetworkManager::~NetworkManager() {
    close(sockTCP);
    close(sockUDP);
}

/**------------------------------------------------------------------------------
Method: run

Date: February. 1, 2017

Designer: Brody McCrone

Programmer: Brody McCrone

Interface: void run(const std::string ip, const std::string username)
ip: IP of the server.
username: Client's username.

Returns:
void

Notes:
Sets NetworkManager state to INITIALIZING and start a thread for the NetworkManager
to run on.
-------------------------------------------------------------------------------*/
void NetworkManager::run(const std::string ip, const std::string username) {
    state = NetworkState::INITIALIZING;
    const in_addr_t serverIP = inet_addr(ip.c_str());
    std::thread TCPThread(&NetworkManager::initTCPClient, this, serverIP, username);
    TCPThread.detach();
}

/*------------------------------------------------------------------------------
* Function: runUDPClient
*
* Date: February. 1, 2017
* Modified: April 5, 2017 (Brody McCrone)
*
* Designer: Eva YU
*
* Programmer: Eva Yu
*
* Interface: void runUDPClient()
*
* Returns:
* void
*
* Notes:
* directs the udp thread loop to game sync de packetizer
-------------------------------------------------------------------------------*/
void NetworkManager::runUDPClient(const in_addr_t serverIP) {
    servUDPAddr = createAddress(serverIP, htons(LISTEN_PORT_UDP));
    servUDPAddrLen = sizeof(struct sockaddr_in);

    sockUDP = createSocket(true, false);
    bindSocket(sockUDP, INADDR_ANY, LISTEN_PORT_UDP);

    ip_mreq mreq;
    memset(&mreq, 0, sizeof(mreq));
    mreq.imr_interface.s_addr = INADDR_ANY;
    if (inet_pton(AF_INET, MULTICAST_ADDR.c_str(), &mreq.imr_multiaddr) != 1) {
        perror("inet_pton");
        exit(1);
    }
    if (setsockopt(sockUDP, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq)) == -1) {
        perror("setsockopt");
        exit(1);
    }

    char buffer[SYNC_PACKET_MAX];
    for(;;) {
        const int packetSize = readUDPSocket(buffer, SYNC_PACKET_MAX);
        if (state < NetworkState::GAME_STARTED) {
            state = NetworkState::GAME_STARTED;
        }
        parseGameSync(buffer, packetSize);
        memset(buffer, 0, SYNC_PACKET_MAX);
    }
}

/**------------------------------------------------------------------------------
* Method: initTCPClient
*
* Date: February. 1, 2017
* Modified: April 5, 2017 (Brody McCrone)
*
* Designer: Brody McCrone
*
* Programmer: Eva Yu
*
* Interface: void initTCPClient(const in_addr_t serverIP, const std::string username)
*
* Notes:
* Sets up TCP Sockets and makes TCP Connection with server. If Connection cannot
* be made it sets the state of the NetMan to FAILED_TO_CONNECT and stops running.
* Once connection is made the UDP client is started in its own thread and the TCP
* client continues in runTCPClient.
*-------------------------------------------------------------------------------*/
void NetworkManager::initTCPClient(const in_addr_t serverIP, const std::string username) {
    sockTCP = createSocket(false, false);
    bindSocket(sockTCP, INADDR_ANY, htons(LISTEN_PORT_TCP));

    if (!connectSocket(sockTCP, createAddress(serverIP, htons(LISTEN_PORT_TCP)))) {
        state = NetworkState::FAILED_TO_CONNECT;
    } else {
        state = NetworkState::CONNECTED;
        std::thread UDPThread(&NetworkManager::runUDPClient, this, serverIP);
        UDPThread.detach();
        runTCPClient(username);
    }
}

void NetworkManager::runTCPClient(const std::string username) {
    logv("Username in runTCPClient: %s\n", username.c_str());
    handshake(username);
    waitRecvId();

    /***********************************************************************/
    /*FOLLOWING TO BE REMOVED WHEN GAME TCP MESSAGE BOX IS INTEGRATED INTO GAME */
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

        if (select(sockTCP + 1, &readSet, nullptr, nullptr, nullptr) < 0) {
            perror("select");
            exit(1);
        }

        if (FD_ISSET(STDIN, &readSet)) {
            string str;
            getline(cin, str);
            writeTCPSocket(buffsend, packControlMsg(buffsend, STD_BUFFSIZE, str.c_str(), myid));
        }

        if (FD_ISSET(sockTCP, &readSet)) {
            if ((bytesRead = readTCPSocket(buffrecv, STD_BUFFSIZE)) == 0) {
                logv("TCP Copnnection closed by server. (Bytes read == 0)\n");
                break;
            }

            parseControlMsg(buffrecv, bytesRead);
        }
    }
    /***********************************************************************/
    close(sockTCP);
}

/**------------------------------------------------------------------------------
* Function: handshake
*
* Date: FEB. 01, 2017
*
* Revisions:
* Version 1.0 - [EY] - 2016/FEB/01 - Created Function
*
* Designer: Brody Mccrone & Eva Yu
*
* Programmer: Eva Yu
*
* Interface: int handshake(const std::string username)
* const std::string username -- username associated with the user
*
* Notes:
* sends usernae server without a user id
  -------------------------------------------------------------------------------*/
void NetworkManager::handshake(const std::string username) const {
    char sendline[STD_BUFFSIZE]{0};
    //send the username to server.
    writeTCPSocket(sendline, packControlMsg(sendline, STD_BUFFSIZE, username.c_str()));
}

/**------------------------------------------------------------------------------
* Function: waitRecvId
*
* Date: FEB. 01, 2017
*
* Revisions:
* Version 1.0 - [EY] - 2016/FEB/01 - Created Function
*
* Designer: Brody Mccrone & Eva Yu
*
* Programmer: Eva Yu
*
* Interface: void waitRecvId()
*
* Notes:
* gets the id from sending user name and stores id
  -------------------------------------------------------------------------------*/
void NetworkManager::waitRecvId() {
    char buffrecv[STD_BUFFSIZE];
    parseControlMsg(buffrecv, readTCPSocket(buffrecv, STD_BUFFSIZE));
    const int32_t *idp = reinterpret_cast<const int32_t *>(buffrecv);
    myid = *idp; // storing ID
}

/**------------------------------------------------------------------------------
 * Function: writeTCPSocket
 *
 * Date: FEB. 01, 2017
 *
 * Revisions:
 * Version 1.0 - [EY] - 2016/FEB/01 - Created Function
 * Version 2.0 - [Brody McCrone] - 2017/APR/05 - swapped loop for assert.
 *
 * Designer: Eva Yu
 *
 * Programmer: Eva Yu
 *
 * Interface: int writeTCPSocket(const char * msg, int len)
 * const char -- the message to write to the TCP socket
 * int len* the length of the message to write
 *
 * Notes:
 * wrapper function writes a message to the TCP Socket
  -------------------------------------------------------------------------------*/
void NetworkManager::writeTCPSocket(const char *buf, const int len) const {
    int res = send(sockTCP, buf, len, 0);
    if (res < 0) {
        perror("send");
        exit(1);
    }
    /* this assertion is here because there were concerns that our send would
    exceed an MTU and wouldn't fully send. */
    assert(res == len);
}

/**------------------------------------------------------------------------------
Method: readTCPSocket

Date: February. 1, 2017

Revisions:
Version, Date and Description

Designer: Eva Yu

Programmer: Brody McCrone

Interface: int readTCPSocket (char * buf, int len)
buf: Buffer to store read result in.
len: Amount of characters to read, can be no more than buffer size.

Returns:
-1: error (check errno for further info)
0: connection was closed.
len: read was successful.

Notes:
This read method reads the amount specified by the param len from the TCP
socket stored as a private member of the NetworkManager.
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
 Method: writeUDPSocket

Date: Feb. 7, 2017
modified:
April 1 (Brody McCrone) - Swapped loop for assert and added running assert

Revisions:
Version, Date and Description

Designer: Brody McCrone

Programmer: Brody McCrone

Params:
buf: Data to be sent.
len: Amount of data. Must be equal to or less than buf size.
servAddr: Address of receiver.

Returns:
void

Notes:
Sends buf to servAddr.
-------------------------------------------------------------------------------*/
void NetworkManager::writeUDPSocket(const char *buf, const int len) const {
    /* NetworkManager::run should be called before this is writeUDPSocket is called. */
    assert(state != NetworkState::NOT_RUNNING);

    const int res = sendto(sockUDP, buf, len, 0,
        reinterpret_cast<sockaddr *>(const_cast<sockaddr_in *>(&servUDPAddr)),
        servUDPAddrLen);

    if (res < 0) {
        perror("read");
        exit(1);
    }

    /* this assertion is here because there were concerns that our send would exceed an MTU and
       wouldn't fully send. */
    assert(res == len);
}


/**------------------------------------------------------------------------------
Method: readUDPSocket

Date: Feb. 7, 2017

Revisions:
Version, Date and Description

Designer: Brody McCrone

Programmer: Brody McCrone

Params:
buf: Buffer to store data received in.
len: Amount of data to receive. Must be less than or equal to buf size.
servAddr: Address of sender.

Returns:
void

Notes:
Receives data from servAddr and stores it in buf.
-------------------------------------------------------------------------------*/
int NetworkManager::readUDPSocket(char *buf, const int len) const {
    sockaddr_in recvAddr;
    socklen_t recvAddrLen = sizeof(recvAddr);
    memset(&recvAddr, 0, recvAddrLen);
    const int res = recvfrom(sockUDP, buf, len, 0, (struct sockaddr *)&recvAddr, &recvAddrLen);
    if (res < 0) {
        perror("recvfrom");
        exit(1);
    }

    return res;
}

/**------------------------------------------------------------------------------
* Function: connectSocket
*
* Date: FEB. 01, 2017
*
* Revisions:
* Version 1.0 - [EY] - 2016/FEB/01 - Created Function
*
* DEsigner: EY
*
* Programmer: Eva Yu
*
* Interface: int connectSocket(const int sock, const sockaddr_in& addr)
* const int sock -- socket
* const sockar_in& addr -- address to associate with socket
*
* Returns:
* true: Successfully made connection.
* false: Failed to make connection.
*
* Notes:
* Connects a TCP Socket, sock, to an address, addr.
  -------------------------------------------------------------------------------*/
bool NetworkManager::connectSocket(const int sock, const sockaddr_in& addr) {
    if ((connect(sock, (struct sockaddr *)&addr, sizeof(sockaddr_in))) < 0) {
        perror("connect");
        return false;
    }
    return true;
}

/**------------------------------------------------------------------------------
* Function: createAddress
*
* Date: FEB. 01, 2017
*
* Revisions:
* Version 1.0 - [EY] - 2016/FEB/01 - Created Function
*
* Designer: EY
*
* Programmer: Eva Yu
*
* Interface: sockaddr_in createAddress(const int sock, const sockaddr_in& addr)
* const int sock -- socket
* const sockar_in& addr -- address to associate with socket
*
* Returns:
* the addr structure
*
* Notes:
* Create Address Wrapper.
  -------------------------------------------------------------------------------*/
sockaddr_in NetworkManager::createAddress(const in_addr_t ip, const int port) {
    sockaddr_in addr;
    memset(&addr, '0', sizeof(sockaddr_in));
    addr.sin_family = AF_INET;
    addr.sin_port = port;
    addr.sin_addr.s_addr = ip;
    return addr;
}
