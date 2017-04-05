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

/**
 * Server side static player id generator.
 * This id is sent by the server to each new client.
 * This counter is created to prevent conflicting ids for clients.
 * John Agapeyev March 19
 */
int32_t getPlayerId() {
    static std::atomic<int32_t> counter{-1};
    return ++counter;
}

/**
 * Binds a socket to a given ip and port.
 * The ip and port are in network byte order.
 * It then returns a servaddr struct containing the address the socket was bound to.
 * John Agapeyev March 19
 */
sockaddr_in bindSocket(const int sock, const unsigned long ip, const unsigned short port) {
    struct sockaddr_in servaddrudp;
    memset(&servaddrudp, 0, sizeof(servaddrudp));
    servaddrudp.sin_family = AF_INET;
    servaddrudp.sin_addr.s_addr = htonl(ip);
    servaddrudp.sin_port = htons(port);

    if ((bind(sock, reinterpret_cast<sockaddr *>(&servaddrudp), sizeof(servaddrudp))) == -1) {
        perror("Bind");
        exit(1);
    }
    return servaddrudp;
}

/**
 * Creates a socket, choosing a TCP or UDP socket, and optionally making it non-blocking.
 * John Agapeyev March 19
 */
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

void fillMulticastAddr(sockaddr_in& addr) {
    memset(&addr, 0, sizeof(addr));

    if (inet_pton(AF_INET, MULTICAST_ADDR.c_str(), &addr.sin_addr) != 1) {
        perror("inet_pton");
        exit(3);
    }

    addr.sin_family = AF_INET;
    addr.sin_port = htons(LISTEN_PORT_UDP);
}

/**
 * Method called when the game should start.
 * This is called when either a /start command is received over TCP, or all players are ready to play.
 * It launches the game logic loop in a throwaway thread.
 * When the intial game has loaded, it will disable the spinlock in this method, allowing the
 * server to start reading and processing UDP packets.
 * This spinlock is done to prevent a situation where a player has "moved" before they have been created
 * or otherwise defined. This is mostly to prevent any issues, since it should theoretically never happen.
 * John Agapeyev March 19
 */
void transitionToGameStart() {
    logv("Starting the game\n");
    close(listenSocketTCP);
    std::thread(startGame).detach();
    //Spinlock
    while (!isGameRunning.load());
    listenUDP(listenSocketUDP, INADDR_ANY, listen_port_udp);
}

/**
 * Sends a message to all clients except for the one with the provided id.
 * It serializes the id into the buffer and adds the appropriate header bytes before
 * sending it off to the clients.
 * John Agapeyev March 19
 */
void sendTCPClientMessage(const int32_t id, const bool isConnectMessage, const char *mesg, const size_t mesgSize) {
    char *outBuff;
    if ((outBuff = reinterpret_cast<char *>(malloc(mesgSize + TCP_HEADER_SIZE + 1))) == nullptr) {
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
 * Processes a received TCP message according to the following format.
 * buff[4] will be either T or C
 * T - Text typed by the user, either server commands or text chat
 * C - Connection information like lobby contents, people being ready, etc.
 * Server ignores messages marked as connection as it is the only one that sends them
 * Server sends T & C messages and ignores flag on receive
 * Client sends T messages and receives T&C messages
 * John Agapeyev March 19
 */
void processTCPMessage(const int sock, const char *buff, const size_t nbytes) {
    //Convert first 4 chars to 32 bit int representing id
    const int32_t idReceived = reinterpret_cast<const int32_t *>(buff)[0];

    logv("Read packet with id: %d\n", idReceived);

    for (auto& it : clientList) {
        if (it.second.entry.sock == sock) {
            if (!it.second.hasSentUsername) {
                processClientUsername(sock, buff, it);
            } else {
                if (buff[5] == '/') {
                    processCommandMessage(idReceived, buff, nbytes);
                } else {
                    //Regular chat message - processing TBD
                    logv("Server received chat message: %s\n", buff + 4);
                    sendTCPClientMessage(idReceived, false, buff + 4, nbytes - 4);
                }
            }
            break;
        }
    }
}

/**
 * Create the epoll file descriptor for use in the TCP and UDP read loops.
 * John Agapeyev March 19
 */
int createEpollFD() {
    int epollfd;
    if ((epollfd = epoll_create1(0)) == -1) {
        perror("Epoll create");
        exit(1);
    }
    return epollfd;
}

/**
 * Adds a socket to a epoll descriptor.
 * The notifications requested are defined in the epoll_event struct that is passed in.
 * John Agapeyev March 19
 */
void addEpollSocket(const int epollfd, const int sock, epoll_event *ev) {
    if ((epoll_ctl(epollfd, EPOLL_CTL_ADD, sock, ev)) == -1) {
        perror("epoll_ctl");
        exit(1);
    }
}

/**
 * Sets a given socket to non-blocking.
 * This is mainly done for newly connected TCP client sockets as
 * the main sockets are created in non-blocking mode already.
 * John Agapeyev March 19
 */
void setSockNonBlock(const int sock) {
    if (fcntl(sock, F_SETFL, O_NONBLOCK) == -1) {
        perror("fcntl");
        close(sock);
        exit(1);
    }
}

/**
 * Method called when a new TCP client has joined.
 * It adds the client to the client list as well as adding it
 * to the epoll descriptor for further notifications.
 * John Agapeyev March 19
 */
void handleIncomingTCP(const int epollfd) {
    logv("Accepting player\n");
    if (isGameRunning.load()) {
        logv("Game has already started so a client cannot be added\n");
        if ((epoll_ctl(epollfd, EPOLL_CTL_DEL, listenSocketTCP, nullptr)) == -1) {
            perror("epoll_ctl");
            exit(1);
        }
        close(listenSocketTCP);
        return;
    }

    sockaddr_in addr;
    socklen_t addrLen = sizeof(addr);
    int clientSock = accept(listenSocketTCP, reinterpret_cast<sockaddr *>(&addr), &addrLen);
    if (clientSock == -1) {
        if (errno == EAGAIN || errno == EWOULDBLOCK) {
            return;
        }
        perror("Accept");
        return;
    }

    setSockNonBlock(clientSock);

    if (getpeername(clientSock, reinterpret_cast<sockaddr *>(&addr), &addrLen) == -1) {
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
    cli.entry.addr.sin_port = htons(listen_port_udp);
    cli.entry.sock = clientSock;
    cli.hasSentUsername = false;
    cli.isPlayerReady = false;

    clientList.insert({getPlayerId(), cli});

    logv("New client has joined the server\n");

    addEpollSocket(epollfd, clientSock, &clientEv);
}

/**
 * Method called when a TCP read event has been received.
 * It reads from the socket and sends it off for processing.
 * This method also performs some error handling for the read.
 * John Agapeyev March 19
 */
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

/**
 * Method called when a UDP read notification was received.
 * It reads from the socket and sends it off for processing.
 * John Agapeyev March 19
 */
void readUDP(const int sock, sockaddr *servaddr, socklen_t *servAddrLen) {
    int nmesg;
    if ((nmesg = recvmmsg(sock, udpMesgs, MAX_UDP_PACKET_COUNT, MSG_DONTWAIT, nullptr)) == -1) {
        perror("recvmmsg");
        return;
    }

    logv("Received %d messages\n", nmesg);

#pragma omp parallel for schedule(static) firstprivate(nmesg)
    for (int i = 0; i < nmesg; ++i) {
        processPacket(readBuffers[i]);
    }
}

/**
 * Waits on an epoll descriptor for events to occur.
 * It writes the event data to the epoll event buffere that was passed in.
 * It then returns the number of events generated.
 * John Agapeyev March 19
 */
int waitForEpollEvent(const int epollfd, epoll_event *events) {
    int nevents;
    if ((nevents = epoll_wait(epollfd, events, MAXEVENTS, -1)) == -1) {
        perror("epoll_wait");
        exit(1);
    }
    return nevents;
}

/**
 * Creates the epoll event buffer for use in the epoll read loops.
 * John Agapeyev March 19
 */
epoll_event *createEpollEventList() {
    epoll_event *events;
    if (!(events = reinterpret_cast<epoll_event *>(calloc(MAXEVENTS, sizeof(epoll_event))))) {
        perror("Calloc failure");
        exit(1);
    }
    return events;
}

/**
 * Processes a received username.
 * Due to the nature of TCP, a new client will be conencted, and then send its username seperately.
 * This method registers the username and updates its entry in the client list.
 * After reading the username, it sends the client back its username with its generated id.
 * Once the client has been updated, it then messages all other clients, informing them of a newly connected player.
 * yPos is made static to ensure all clients start at different positions instead of being on top of one another
 * John Agapeyev March 19
 */
void processClientUsername(const int sock, const char *buff, std::pair<const int32_t, PlayerJoin>& client) {
    static Base base;
    //Handle initial username read
    client.second.hasSentUsername = true;
    client.second.isPlayerReady = false;
    strncpy(client.second.entry.username, buff + TCP_HEADER_SIZE + 1, NAMELEN);
    strcat(client.second.entry.username, "\0");
    logv("Server received username: %s\n", client.second.entry.username);

    const size_t bufferSize = NAMELEN + TCP_HEADER_SIZE + 1;
    char outBuff[bufferSize];
    memset(outBuff, '\0', bufferSize);
    int32_t *id = reinterpret_cast<int32_t *>(outBuff);
    *id = client.first;
    outBuff[4] = 'C';
    outBuff[5] = '/';

    strncpy(outBuff + TCP_HEADER_SIZE + 1, client.second.entry.username, NAMELEN);

    gm->createMarine(client.first);
    auto& marine = gm->getMarine(client.first).first;
    const auto& spawnPoint = base.getSpawnPoint();
    marine.setPosition(spawnPoint.first, spawnPoint.second);

    //Send client their allocated id and username
    if (!rawClientSend(sock, outBuff, bufferSize)) {
        return;
    }

    updateClientWithCurrentLobby(sock, outBuff, bufferSize);
}

/**
 * Updates the lobby with the username and id of a newly joined player.
 * John Agapeyev March 19
 */
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

/**
 * Wrapper method that sends a message of passed in size over a socket.
 * John Agapeyev March 19
 */
bool rawClientSend(const int sock, const char *outBuff, const size_t bufferSize) {
    if (send(sock, outBuff, bufferSize, 0) < 0) {
        perror("Failed to send client message");
        return false;
    }
    return true;
}

/**
 * Handles command messages for the TCP server.
 * It checks the type of the message, and hands it off to the appropriate handler.
 * John Agapeyev March 19
 */
void processCommandMessage(const int32_t idReceived, const char *buff, const int nbytes) {
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

/**
 * Checks whether all clients are currently ready to play.
 * John Agapeyev March 19
 */
bool checkIfAllClientsAreReady() {
    return std::all_of(clientList.cbegin(), clientList.cend(),
            [](const auto& elem){return elem.second.isPlayerReady;});
}

/**
 * Handles a ready message from a client.
 * It updates the client list ready status, informs all other clients,
 * and then checks if the game shoudl be started.
 * John Agapeyev March 19
 */
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

/**
 * Handles an unready message from a client.
 * It chaanges the ready status of the client, and then updates the
 * lobby with the newly updated status.
 * John Agapeyev March 19
 */
void processUnreadyMessage(const int32_t idReceived) {
    if (clientList.count(idReceived)) {
        clientList[idReceived].isPlayerReady = false;
        logv("Player id %d is NOT ready to start\n", idReceived);
        sendTCPClientMessage(idReceived, false, "unready", 7);
    } else {
        perror("Client not found in list");
    }
}
