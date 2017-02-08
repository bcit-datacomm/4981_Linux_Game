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
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include "UDPSocket.hpp"

/*--------------------------------------------------------------------------
DESTRUCTOR: UDPSocket

DATE: Feb. 7, 2017

REVISIONS:
Version, Date and Description

DESIGNER: Brody McCrone

PROGRAMMER: Brody McCrone

PARAMS:

NOTES:
Opens socket and stores file descriptor.
--------------------------------------------------------------------------*/
UDPSocket::UDPSocket() {
    if((_sockUDP = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket");
        exit(1);
    }

    int truth = 1;
    if(setsockopt(_sockUDP, SOL_SOCKET, SO_REUSEADDR, &truth, sizeof(int)) < 0) {
        perror("setsockopt");
        exit(1);
    }
}

/*--------------------------------------------------------------------------
DESTRUCTOR: ~UDPSocket

DATE: Feb. 7, 2017

REVISIONS:
Version, Date and Description

DESIGNER: Brody McCrone

PROGRAMMER: Brody McCrone

PARAMS:

NOTES:
Closes the socket.
--------------------------------------------------------------------------*/
UDPSocket::~UDPSocket() {
    close(_sockUDP);
}

/*--------------------------------------------------------------------------
METHOD: bindTo

DATE: February. 7, 2017

REVISIONS:
Version, Date and Description

DESIGNER: Brody McCrone

PROGRAMMER: Brody McCrone

PARAMS:
port: The port to bind the socket to.

RETURNS:
void

NOTES:
Binds the socket to the specified port. (Wrapper method for bind function)
--------------------------------------------------------------------------*/
void UDPSocket::bindTo(int port) {
    struct sockaddr_in myAddress;
    memset((char *) &myAddress, 0, sizeof(myAddress));
    myAddress.sin_family = AF_INET;
    myAddress.sin_port = htons(port);
    myAddress.sin_addr.s_addr = INADDR_ANY;

    if(bind(_sockUDP, (struct sockaddr *)&myAddress, sizeof(myAddress)) < 0) {
        perror("bind");
        exit(1);
    }
}

/*--------------------------------------------------------------------------
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
void UDPSocket::sendTo(const char *buf, int len, struct sockaddr_in servAddr) {
    socklen_t addrLen = sizeof(servAddr);
    int res = 0;
	int ttlsent = 0, bytesleft = len;
	while (ttlsent < len) {
        if ((res = sendto(_sockUDP, buf + ttlsent, bytesleft, 0,
            (struct sockaddr *)&servAddr, addrLen)) < 0 ) {
			perror("read");
            exit(1);
		}
		ttlsent += res;
		bytesleft -= res;
	}
}

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
void UDPSocket::receiveFrom(char *buf, int len, struct sockaddr_in servAddr) {
    socklen_t addrLen = sizeof(servAddr);
    int res = 0;
	int ttlsent = 0, bytesleft = len;
	while (ttlsent < len) {
		if ((res = recvfrom(_sockUDP, buf + ttlsent, bytesleft, 0,
            (struct sockaddr *)&servAddr, &addrLen)) < 0 ) {
			perror("recvfrom");
            exit(1);
		}
		ttlsent += res;
		bytesleft -= res;
	}
}
