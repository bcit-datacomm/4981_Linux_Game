#include "UDPSocket.hpp"
#include <sys/socket.h>

#define BUFFER_SIZE 1024

struct Packet {
    char buf[BUFFER_SIZE];
    int len;
};

void init(const char *, int);
void sendloop(UDPSocket *, struct sockaddr_in);
const char *retrieve();
Packet packetize(const char *);
void send(Packet pkt, UDPSocket sendSockUDP, struct sockaddr_ins servAddr);
void recvloop(UDPSocket *, struct sockaddr_in);
char *depacketize(char *);
void process(char *);
