#ifndef SERVER_H
#define SERVER_H

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <cstdarg>
#include <climits>
#include <atomic>
#include <unordered_map>

//Fix issue where Brody has out-of-date system that doesn't have epoll exclusive
#ifndef EPOLLEXCLUSIVE
#define EPOLLEXCLUSIVE 1u << 28
#endif

//Temp variable to represent client count
#define CLIENT_COUNT 10

#define LISTEN_PORT_UDP 35222
#define LISTEN_PORT_TCP 35223
#define TICK_RATE 20
#define IN_PACKET_SIZE USHRT_MAX
#define OUT_PACKET_SIZE USHRT_MAX
#define SYNC_IN 32 //name padded with nulls
#define NAMELEN 32 //same as above but kept seperate for clarity of purpose
#define SYNC_OUT 33 //name padded with nulls + id
#define OPT_STRING "ni:p:hl:L:c:evo:"
#define MAX_PORT 65535
#define LISTENQ 25 //although many kernals define it as 5 usually it can support many more
#define MAXEVENTS 100 //Maximum number of simultaneous epoll events
#define MAX_UDP_PACKET_COUNT 500 //Maximum number of packets to read from the UDP socket in one go
#define TCP_HEADER_SIZE 5 //4 bytes for int32_t one byte for C/T char
#define MULTICAST_ADDR "226.23.41.86"

struct ClientEntry {
    char username[NAMELEN + 1];
    sockaddr_in addr;
    int sock;
};

struct PlayerJoin {
    ClientEntry entry;
    bool hasSentUsername;
    bool isPlayerReady;
};

extern char outputPacket[OUT_PACKET_SIZE];
extern int listenSocketUDP;
extern int listenSocketTCP;
extern int sendSocketUDP;
extern sockaddr_in sendAddrUDP;
extern std::unordered_map<int32_t, PlayerJoin> clientList;
extern std::atomic_bool isGameRunning;
extern int listen_port_udp;
extern int listen_port_tcp;
extern size_t client_count;
extern int outputLength;
extern char readBuffers[MAX_UDP_PACKET_COUNT][IN_PACKET_SIZE];
extern iovec iovecs[MAX_UDP_PACKET_COUNT];
extern mmsghdr udpMesgs[MAX_UDP_PACKET_COUNT];

void initSync(const int sock);
void processPacket(const char *data);
void genOutputPacket();
void sendSyncPacket(const int sock);
void listenForPackets(const sockaddr_in servaddr);
void listenTCP(const int socket, const unsigned long ip, const unsigned short port);
void listenUDP(const int socket, const unsigned long ip, const unsigned short port);
#endif
