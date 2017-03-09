#ifndef SERVER_H
#define SERVER_H

#include <netinet/in.h>
#include <cstdarg>
#include <atomic>

//Temp variable to represent client count
#define CLIENT_COUNT 10

#define LISTEN_PORT_UDP 35222
#define LISTEN_PORT_TCP 35223
#define TICK_RATE 20
#define IN_PACKET_SIZE 1024 //TBD
#define OUT_PACKET_SIZE 1024 //TBD
#define SYNC_IN 32 //name padded with nulls
#define NAMELEN 32 //same as above but kept seperate for clarity of purpose
#define SYNC_OUT 33 //name padded with nulls + id
#define OPT_STRING "hl:L:c:v"
#define MAX_PORT 65535
#define LISTENQ 25 //although many kernals define it as 5 usually it can support many more
#define MAXEVENTS 100 //although many kernals define it as 5 usually it can support many more

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

extern const long long microSecPerTick;
extern char outputPacket[OUT_PACKET_SIZE];
extern int listenSocketUDP;
extern int listenSocketTCP;
extern int sendSocketUDP;
extern std::unordered_map<int32_t, PlayerJoin> clientList;
extern std::atomic_bool isGameRunning;

void initSync(int sock);
void processPacket(const char *data);
void genOutputPacket();
void sendSyncPacket(int sock);
void listenForPackets(const struct sockaddr_in servaddr);
void alarmHandler(int signo);
void startTimer();
void listenTCP(int socket, unsigned long ip, unsigned short port);
void listenUDP(int socket, unsigned long ip, unsigned short port);
int createSocket(bool useUDP, bool nonblocking);
int32_t getPlayerId();
void transitionToGameStart();
void sendTCPClientMessage(const int32_t id, const bool isConnectMessage, const char *mesg, const size_t mesgSize);
void processTCPMessage(const char *buff, const size_t nbytes, int sock);

//off by default
extern bool verbose;
//logging for statuses when in verbose mode
void logv(const char *msg, ...);
#endif
