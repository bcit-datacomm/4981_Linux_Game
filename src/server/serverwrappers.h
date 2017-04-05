#ifndef SERVER_WRAPPERS_H
#define SERVER_WRAPPERS_H

#include <arpa/inet.h>
#include <sys/epoll.h>

#include "server.h"

//Simple wrappers that wrap api calls
int createSocket(const bool useUDP, const bool nonblocking);
sockaddr_in bindSocket(const int sock, const unsigned long ip, const unsigned short port);
int createEpollFD();
void addEpollSocket(const int epollfd, const int sock, epoll_event *ev);
void setSockNonBlock(const int sock);
void fillMulticastAddr(sockaddr_in& addr);
epoll_event *createEpollEventList();

//Game related calls
int32_t getPlayerId();
void transitionToGameStart();

//More network oriented functions
void sendTCPClientMessage(const int32_t id, const bool isConnectMessage, const char *mesg, const size_t mesgSize);
void processTCPMessage(const int sock, const char *buff, const size_t nbytes);
void handleIncomingTCP(const int epollfd);
void readTCP(const int sock);
void readUDP(const int sock, sockaddr *servaddr, socklen_t *servAddrLen);
int waitForEpollEvent(const int epollfd, epoll_event *events);
bool rawClientSend(const int sock, const char *outBuff, const size_t bufferSize);

//Networking handshake calls
void processClientUsername(const int sock, const char *buff, std::pair<const int32_t, PlayerJoin>& client);
void processCommandMessage(const int32_t idReceived, const char *buff, const int nbytes);
bool checkIfAllClientsAreReady();
void processReadyMessage(const int32_t idReceived);
void processUnreadyMessage(const int32_t idReceived);
void updateClientWithCurrentLobby(const int sock, char *outBuff, const size_t bufferSize);

#endif
