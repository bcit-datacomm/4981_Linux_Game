#include <cstdint>
#include <cstdlib>
#include <cstdio>
#include <thread>
#include <algorithm>

#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/epoll.h>

#include "server.h"
#include "servergamestate.h"
#include "serverwrappers.h"

int32_t getPlayerId() {
    static std::atomic<int32_t> counter{0};
    return ++counter;
}

sockaddr_in bindSocket(const int sock, const unsigned long ip, const unsigned short port) {
    struct sockaddr_in servaddrudp;
    memset(&servaddrudp, 0, sizeof(servaddrudp));
    servaddrudp.sin_family = AF_INET;
    servaddrudp.sin_addr.s_addr = htonl(ip);
    servaddrudp.sin_port = htons(port); 

    if ((bind(sock, (struct sockaddr *) &servaddrudp, sizeof(servaddrudp))) == -1) {
        perror("Bind");
        exit(1);
    }
    return servaddrudp;
}

int createSocket(const bool useUDP, const bool nonblocking) {
    int sock = socket(AF_INET, ((useUDP) ? SOCK_DGRAM : SOCK_STREAM) | (nonblocking * SOCK_NONBLOCK), 0);
    if (sock == -1) {
        perror("Create socket");
        exit(1);
    }
    int enable = 1;
    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0) {
        perror("setsockopt(SO_REUSEADDR) failed");
        exit(1);
    }
    return sock;
}

void logv(const char *msg, ...) {
    if (!verbose) {
        return;
    }
    va_list args;
    va_start(args, msg);
    vprintf(msg, args);
    va_end(args);
    fflush(stdout);
}

void transitionToGameStart() {
    logv("Starting the game\n");
    std::thread(startGame).detach();
    //Spinlock
    while (!isGameRunning.load());
    listenUDP(listenSocketUDP, INADDR_ANY, listen_port_udp);
}

void sendTCPClientMessage(const int32_t id, const bool isConnectMessage, const char *mesg, const size_t mesgSize) {
    char *outBuff;
    if ((outBuff = (char *) malloc(mesgSize + TCP_HEADER_SIZE + 1)) == nullptr) {
        perror("Malloc failure");
        exit(1);
    }
    memset(outBuff, '\0', mesgSize + TCP_HEADER_SIZE + 1);
    reinterpret_cast<int32_t *>(outBuff)[0] = id;
    outBuff[4] = (isConnectMessage) ? 'C' : 'T';
    outBuff[5] = '/';

    strncpy(outBuff + TCP_HEADER_SIZE + 1, mesg, mesgSize);

    for (const auto& clients : clientList) {
        if (clients.first != id && clients.second.hasSentUsername) {
            rawClientSend(clients.second.entry.sock, outBuff, mesgSize + TCP_HEADER_SIZE + 1);
        }
    }
    free(outBuff);
}

/*
 * buff[4] will be either T or C
 * T - Text typed by the user, either server commands or text chat
 * C - Connection information like lobby contents, people being ready, etc.
 * Server ignores messages marked as connection as it is the only one that sends them
 * Server sends T & C messages and ignores flag on receive
 * Client sends T messages and receives T&C messages
 */
void processTCPMessage(const int sock, const char *buff, const size_t nbytes) {
    //Convert first 4 chars to 32 bit int representing id
    const int32_t idReceived = reinterpret_cast<const int32_t *>(buff)[0];

    logv("Read packet with id: %d\n", idReceived);

    for (const auto& it : clientList) {
        if (it.second.entry.sock == sock) {
            if (!it.second.hasSentUsername) {
                processClientUsername(sock, buff, it);
            } else {
                if (buff[5] == '/') {
                    processCommandMessage(idReceived, buff, nbytes);
                } else {
                    //Regular chat message - processing TBD
                    logv("Server received: %s\n", buff + 4);
                }
            }
            break;
        }
    }
}

int createEpollFD() {
    int epollfd;
    if ((epollfd = epoll_create1(0)) == -1) {
        perror("Epoll create");
        exit(1);
    }
    return epollfd;
}

void addEpollSocket(const int epollfd, const int sock, epoll_event *ev) {
    if ((epoll_ctl(epollfd, EPOLL_CTL_ADD, sock, ev)) == -1) {
        perror("epoll_ctl");
        exit(1);
    }
}

void setSockNonBlock(const int sock) {
    if (fcntl(sock, F_SETFL, O_NONBLOCK) == -1) {
        perror("fcntl");
        close(sock);
        exit(1);
    }
}

void handleIncomingTCP(const int epollfd) {
    logv("Accepting player\n");

    sockaddr_in addr;
    socklen_t addrLen = sizeof(addr);
    int clientSock = accept(listenSocketTCP, (sockaddr *) &addr, &addrLen);
    if (clientSock == -1) {
        if (errno == EAGAIN || errno == EWOULDBLOCK) {
            return;
        }
        perror("Accept");
        return;
    }

    setSockNonBlock(clientSock);

    if (getpeername(clientSock, (sockaddr *) &addr, &addrLen) == -1) {
        perror("GetPeerName");
        close(clientSock);
        return;
    }

    epoll_event clientEv;
    memset(&clientEv, 0, sizeof(epoll_event));
    clientEv.events = EPOLLIN | EPOLLET | EPOLLEXCLUSIVE;
    clientEv.data.fd = clientSock;

    PlayerJoin cli{0};
    cli.entry.addr = addr;
    cli.entry.sock = clientSock;
    cli.hasSentUsername = false;
    cli.isPlayerReady = false;

    clientList.insert({getPlayerId(), cli});

    logv("New client has joined the server\n");

    addEpollSocket(epollfd, clientSock, &clientEv);
}

void readTCP(const int sock) {
    int nbytes;
    char buff[IN_PACKET_SIZE];
    logv("Reading data\n");
    while ((nbytes = recv(sock, buff, IN_PACKET_SIZE - 1, 0)) > 0) {
        //Handle message
        if (nbytes < 4) {
            logv("Packet read was too small\n");
            continue;
        }
        buff[nbytes] = '\0';
        processTCPMessage(sock, buff, nbytes);
    }
    if (nbytes == -1) {
        if (errno == EAGAIN || errno == EWOULDBLOCK) {
            return;
        }
        perror("Packet read failure");
    } else {
        logv("Client %d closed connection\n", static_cast<int>(sock));
    }
    close(sock);
}

void readUDP(const int sock, sockaddr *servaddr, socklen_t *servAddrLen) {
    char buff[IN_PACKET_SIZE];
    int nbytes;
    while ((nbytes = recvfrom(sock, buff, IN_PACKET_SIZE, 
                    0, servaddr, servAddrLen)) > 0) {
        logv("Received %d bytes\n", nbytes);
#pragma omp task
        processPacket(buff);
    }
}

int waitForEpollEvent(const int epollfd, epoll_event *events) {
    int nevents;
    if ((nevents = epoll_wait(epollfd, events, MAXEVENTS, -1)) == -1) {
        perror("epoll_wait");
        exit(1);
    }
    return nevents;
}

epoll_event *createEpollEventList() {
    epoll_event *events;
    if (!(events = (epoll_event *) calloc(MAXEVENTS, sizeof(epoll_event)))) {
        perror("Calloc failure");
        exit(1);
    }
    return events;
}

void processClientUsername(const int sock, const char *buff, const std::pair<int32_t, PlayerJoin>& client) {
    static float yPos = 0;
    //Handle initial username read
    std::pair<int32_t, PlayerJoin> tempMapEntry;
    tempMapEntry = client;
    tempMapEntry.first = getPlayerId();
    tempMapEntry.second.hasSentUsername = true;
    tempMapEntry.second.isPlayerReady = false;
    tempMapEntry.second.entry.addr = client.second.entry.addr;
    tempMapEntry.second.entry.addr.sin_port = htons(listen_port_udp);
    strncpy(tempMapEntry.second.entry.username, buff + TCP_HEADER_SIZE + 1, NAMELEN);
    strcat(tempMapEntry.second.entry.username, "\0");
    logv("Server received username: %s\n", tempMapEntry.second.entry.username);

    //Erase temporary entry in client list
    clientList.erase(clientList.find(client.first));
    //Insert newly compelted entry
    clientList.insert(tempMapEntry);

    const size_t bufferSize = NAMELEN + TCP_HEADER_SIZE + 1;
    char outBuff[bufferSize];
    memset(outBuff, '\0', bufferSize);
    int32_t *id = reinterpret_cast<int32_t *>(outBuff);
    *id = tempMapEntry.first;
    outBuff[4] = 'C';
    outBuff[5] = '/';

    strncpy(outBuff + TCP_HEADER_SIZE + 1, tempMapEntry.second.entry.username, NAMELEN);

    gm->createMarine(tempMapEntry.first);
    gm->getMarine(tempMapEntry.first).setPosition(100, yPos);
    yPos += 150;

    //Send client their allocated id and username
    if (!rawClientSend(sock, outBuff, bufferSize)) {
        return;
    }

    updateClientWithCurrentLobby(sock, outBuff, bufferSize);
}

void updateClientWithCurrentLobby(const int sock, char *outBuff, const size_t bufferSize) {
    //Send new client a list of already existing clients 
    int32_t *id;
    for (const auto& it : clientList) {
        if (it.second.entry.sock != sock && it.second.hasSentUsername) {
            memset(outBuff, '\0', bufferSize);
            id = reinterpret_cast<int32_t *>(outBuff);
            *id = it.first;
            outBuff[4] = 'C';
            outBuff[5] = '/';
            strncpy(outBuff + TCP_HEADER_SIZE + 1, it.second.entry.username, NAMELEN);

            if (!rawClientSend(sock, outBuff, bufferSize)) {
                break;
            }
            outBuff[4] = 'T';
            memset(outBuff + TCP_HEADER_SIZE + 1, '\0', NAMELEN);
            if (it.second.isPlayerReady) {
                strncpy(outBuff + TCP_HEADER_SIZE + 1, "ready", 5);                
            } else {
                strncpy(outBuff + TCP_HEADER_SIZE + 1, "unready", 7);                
            }
            if (!rawClientSend(sock, outBuff, bufferSize)) {
                break;
            }
        }
    }
}

bool rawClientSend(const int sock, const char *outBuff, const size_t bufferSize) {
    if (send(sock, outBuff, bufferSize, 0) < 0) {
        perror("Failed to send client message");
        return false;
    }
    return true;
}

void processCommandMessage(const int32_t idReceived, const char *buff, const int nbytes) {
    //Command message
    if (strncmp(buff + TCP_HEADER_SIZE + 1, "ready", nbytes - (TCP_HEADER_SIZE + 1)) == 0) {
        //Ready command
        processReadyMessage(idReceived);
    } else if (strncmp(buff + TCP_HEADER_SIZE + 1, "unready", nbytes - (TCP_HEADER_SIZE + 1)) == 0) {
        //Unready command
        processUnreadyMessage(idReceived);
    } else if (strncmp(buff + TCP_HEADER_SIZE + 1, "start", nbytes - (TCP_HEADER_SIZE + 1)) == 0) {
        //Start game command
        transitionToGameStart();
    }
}

bool checkIfAllClientsAreReady() {
    return std::all_of(clientList.cbegin(), clientList.cend(), 
            [](const auto& elem){return elem.second.isPlayerReady;});
}

void processReadyMessage(const int32_t idReceived) {
    if (clientList.count(idReceived)) {
        clientList[idReceived].isPlayerReady = true;
        logv("Player id %d is ready to start\n", idReceived);
        sendTCPClientMessage(idReceived, false, "ready", 5);
    } else {
        perror("Client not found in list");
    }
    if (checkIfAllClientsAreReady()) {
        transitionToGameStart();
    }
}

void processUnreadyMessage(const int32_t idReceived) {
    if (clientList.count(idReceived)) {
        clientList[idReceived].isPlayerReady = false;
        logv("Player id %d is NOT ready to start\n", idReceived);
        sendTCPClientMessage(idReceived, false, "unready", 7);
    } else {
        perror("Client not found in list");
    }
}
