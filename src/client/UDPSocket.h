#ifndef UDPCLIENT_HPP
#define UDPCLIENT_HPP

/*------------------------------------------------------------------------------
SOURCE FILE: UDPSocket.cpp

CLASS: UDPSocket

METHODS:
void UDPSocket::bindTo(int port)
void UDPSocket::sendTo(const char *buf, int len, struct sockaddr_in servAddr)
void UDPSocket::receiveFrom(char *buf, int len, struct sockaddr_in servAddr)

DATE: Feb. 7, 2017

REVISIONS:
Version, Date and Description

DESIGNER: Brody McCrone

PROGRAMMER: Brody McCrone

NOTES:
UDPSocket encapsulates a file descriptor for a UDP socket.
------------------------------------------------------------------------------*/

#include <sys/socket.h>
#include <arpa/inet.h>

#define LISTEN_PORT_UDP 35222

class UDPSocket {
public:
    UDPSocket() {};
    UDPSocket(const char *);
    virtual ~UDPSocket();
    void sendToServ(const char *, const int&);
    int recvFromServ(char *, const int&);
private:
    void bindSock();
    int _sockUDP;
    struct sockaddr_in _servAddr;
};

#endif
