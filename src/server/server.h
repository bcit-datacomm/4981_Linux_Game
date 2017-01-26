#ifndef SERVER_H
#define SERVER_H

//Temp variable to represent client count
#define CLIENT_COUNT 10

#define LISTEN_PORT 35222
#define TICK_RATE 20
#define IN_PACKET_SIZE 1024 //TBD
#define OUT_PACKET_SIZE 1024 //TBD
#define OPT_STRING "a:b:c:"
const long long microSecPerTick = (1000 * 1000) / TICK_RATE;
char outputPacket[OUT_PACKET_SIZE];
int listenSocket;
int sendSocket;
struct sockaddr_in clientAddrs[CLIENT_COUNT];

void processPacket(const char *data);
void genOutputPacket();
void sendSyncPacket(int sock);
void listenForPackets(const struct sockaddr_in servaddr);
void alarmHandler(int signo);

#endif
