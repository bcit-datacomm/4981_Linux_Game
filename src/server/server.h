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
#include <mutex>
#include <unordered_map>

//Fix issue where Brody has out-of-date system that doesn't have epoll exclusive
#ifndef EPOLLEXCLUSIVE
#define EPOLLEXCLUSIVE 1u << 28
#endif

//Temp variable to represent client count
static constexpr int CLIENT_COUNT = 10;

static constexpr int LISTEN_PORT_UDP = 35222;
static constexpr int LISTEN_PORT_TCP = 35223;
static constexpr int TICK_RATE = 20;
static constexpr int IN_PACKET_SIZE = USHRT_MAX;
static constexpr int OUT_PACKET_SIZE = USHRT_MAX;
static constexpr int SYNC_IN = 32; //name padded with nulls
static constexpr int NAMELEN = 32; //same as above but kept seperate for clarity of purpose
static constexpr int SYNC_OUT = 33; //name padded with nulls + id
static const std::string OPT_STRING = "ni:p:hl:L:c:evo:";
static constexpr int MAX_PORT = 65535;
static constexpr int LISTENQ = 25; //although many kernals define it as 5 usually it can support many more
static constexpr int MAXEVENTS = 100; //Maximum number of simultaneous epoll events
static constexpr int MAX_UDP_PACKET_COUNT = 500; //Maximum number of packets to read from the UDP socket in one go
static constexpr int TCP_HEADER_SIZE = 5; //4 bytes for int32_t one byte for C/T char
static const std::string MULTICAST_ADDR = "226.23.41.86";

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

extern int listenSocketUDP;
extern int listenSocketTCP;
extern int sendSocketUDP;
extern sockaddr_in sendAddrUDP;
extern std::unordered_map<int32_t, PlayerJoin> clientList;
extern std::atomic_bool isGameRunning;
extern int listen_port_udp;
extern int listen_port_tcp;
extern size_t client_count;
extern char readBuffers[MAX_UDP_PACKET_COUNT][IN_PACKET_SIZE];
extern iovec iovecs[MAX_UDP_PACKET_COUNT];
extern mmsghdr udpMesgs[MAX_UDP_PACKET_COUNT];
extern std::mutex mut;

void initSync(const int sock);
void processPacket(const char *data);
std::string genOutputPacket();
void sendSyncPacket(const int sock);
void listenForPackets(sockaddr_in& servaddr);
void listenTCP(const int socket, const unsigned long ip, const unsigned short port);
void listenUDP(const int socket, const unsigned long ip, const unsigned short port);
#endif
