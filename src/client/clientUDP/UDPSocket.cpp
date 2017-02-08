
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include "UDPSocket.hpp"

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

UDPSocket::~UDPSocket() {
    close(_sockUDP);
}

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
