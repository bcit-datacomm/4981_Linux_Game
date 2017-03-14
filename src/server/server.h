#ifndef SERVER_H
#define SERVER_H

#include <netinet/in.h>
#include <cstdarg>
#include <climits>
#include <atomic>
#include <unordered_map>

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
#define OPT_STRING "hl:L:c:v"
#define MAX_PORT 65535
#define LISTENQ 25 //although many kernals define it as 5 usually it can support many more
#define MAXEVENTS 100 //although many kernals define it as 5 usually it can support many more
#define TCP_HEADER_SIZE 5 //4 bytes for int32_t one byte for C/T char

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
extern std::unordered_map<int32_t, PlayerJoin> clientList;
extern std::atomic_bool isGameRunning;
extern int listen_port_udp;
extern int listen_port_tcp;
extern size_t client_count;
extern int outputLength;

void initSync(const int sock);
void processPacket(const char *data);
void genOutputPacket();
void sendSyncPacket(const int sock);
void listenForPackets(const sockaddr_in servaddr);
void listenTCP(const int socket, const unsigned long ip, const unsigned short port);
void listenUDP(const int socket, const unsigned long ip, const unsigned short port);

//off by default
extern bool verbose;
//logging for statuses when in verbose mode
void logv(const char *msg, ...);
#endif
