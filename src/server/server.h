#ifndef SERVER_H
#define SERVER_H

#include <stdarg.h>
#include <stdbool.h>

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

typedef struct Players{
    int x,y;
    float dx,dy;
    int id;
    char username[NAMELEN];
} Player;

typedef struct Clients{
    Player *player;
    struct sockaddr_in *addr;
} Client;

const long long microSecPerTick = (1000 * 1000) / TICK_RATE;
char outputPacket[OUT_PACKET_SIZE];
int listenSocketUDP,listenSocketTCP;
int sendSocketUDP, sendSocketTCP;
Client *clients;

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

//where each client w/ both socket and user info is stored

//off by default
bool verbose = false;
//logging for statuses when in verbose mode
void logv(const char *msg, ...){
    if(!verbose)
        return;
    va_list args;
    va_start(args, msg);
    vprintf(msg, args);
    va_end(args);
    fflush(stdout);
}
#endif
