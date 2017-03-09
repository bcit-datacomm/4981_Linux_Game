#include <omp.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fcntl.h>
#include <unistd.h>
#include <getopt.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/epoll.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <sys/signal.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <cstdarg>
#include <climits>
#include <unordered_map>
#include <atomic>
#include <thread>

#include "../UDPHeaders.h"
#include "server.h"
#include "servergamestate.h"

//declared here so they can be overriden with flags at run time as needed
int listen_port_udp = LISTEN_PORT_UDP;
int listen_port_tcp = LISTEN_PORT_TCP;
size_t client_count = CLIENT_COUNT;
int outputLength = 0;

const long long microSecPerTick = (1000 * 1000) / TICK_RATE;
char outputPacket[OUT_PACKET_SIZE];
int listenSocketUDP;
int listenSocketTCP;
int sendSocketUDP;
bool verbose = false;
std::unordered_map<int32_t, PlayerJoin> clientList;
std::atomic_bool isGameRunning{false};

int main(int argc, char **argv) {
    setenv("OMP_PROC_BIND", "TRUE", 1);
    setenv("OMP_DYNAMIC", "TRUE", 1);

    int opt;
    while ((opt = getopt(argc, argv, OPT_STRING)) != -1) {
        switch(opt) {
            case 'h':
                printf("usage %s\noptions:\n\t"
                        " -h this message\n\t"
                        "-l the port to listen to for UDP, default 35222\n\t"
                        "-L the port to listen to for TCP, default 35223\n\t"
                        "-c the number of clients to accept max, default 10\n",
                        argv[0]);
                exit(0);
                break;
            case 'l'://listen port UDP
                listen_port_udp = atoi(optarg);
                if(listen_port_udp < 1 || listen_port_udp > MAX_PORT){
                    printf("l must be an integer 0<x<%d\n", MAX_PORT);
                    exit(2);
                }
                break;
            case 'L'://listen port TCP
                listen_port_tcp = atoi(optarg);
                if(listen_port_tcp < 1 || listen_port_tcp > MAX_PORT){
                    printf("L must be an integer 0<x<%d\n",MAX_PORT);
                    exit(2);
                }
                break;
            case 'c'://client count
                client_count = atoi(optarg);
                if(client_count < 0 || client_count > 100){
                    printf("c must be an integer 0<x<100\n");
                    exit(2);
                }
                break;
            case 'v':
                verbose = true;
                break;
        }
    }
    if(listen_port_udp == listen_port_tcp){
        printf("l cannot be the same port as L\n");
        exit(2);
    }

    logv("UDP port: %d, TCP port: %d, Max clients %d\n",
            listen_port_udp,
            listen_port_tcp,
            client_count);

    if ((listenSocketUDP = createSocket(true, true)) == -1) {
        perror("ListenSocket UDP");
        exit(1);
    }
    if ((sendSocketUDP = createSocket(true, true)) == -1) {
        perror("sendSocket UDP");
        exit(1);
    }
    if ((listenSocketTCP = createSocket(false, false)) == -1) {
        perror("ListenSocket TCP");
        exit(1);
    }

    listenTCP(listenSocketTCP, INADDR_ANY, listen_port_tcp);

    logv("Sockets created and bound\n");

    initSync(listenSocketTCP);

    listenUDP(listenSocketUDP, INADDR_ANY, listen_port_udp);
    return 0;
}

void initSync(int sock) {
    int epollfd;
    epoll_event ev;
    epoll_event *events;

    logv("Starting TCP sync\n");

    if (!(events = (epoll_event *) calloc(MAXEVENTS, sizeof(epoll_event)))) {
        perror("Calloc failure");
        exit(1);
    }

    if ((epollfd = epoll_create1(0)) == -1) {
        perror("Epoll create");
        exit(1);
    }

    ev.events = EPOLLIN | EPOLLET | EPOLLEXCLUSIVE;
    ev.data.fd = listenSocketTCP;

    if ((epoll_ctl(epollfd, EPOLL_CTL_ADD, ev.data.fd, &ev)) == -1) {
        perror("epoll_ctl");
        exit(1);
    }

    char buff[USHRT_MAX];
    ssize_t nbytes = 0;
    int nevents = 0;
    for (;;) {
        if ((nevents = epoll_wait(epollfd, events, 100, -1)) == -1) {
            perror("epoll_wait");
            exit(1);
        }
        for (int i = 0; i < nevents; ++i) {
            if (events[i].events & EPOLLERR) {
                perror("Socket error");
                close(events[i].data.fd);
                continue;
            }
            if (events[i].events & EPOLLHUP) {
                //Peer closed the connection
                logv("Peer closed connection\n");
                close(events[i].data.fd);
                continue;
            }
            if (events[i].events & EPOLLIN) {
                if (events[i].data.fd == listenSocketTCP) {
                    logv("Accepting player\n");
                    PlayerJoin cli{0};

                    sockaddr_in addr;
                    socklen_t addrLen;
                    ev.data.fd = accept(listenSocketTCP, (sockaddr *) &addr, &addrLen);

                    logv("New client has joined the server\n");

                    cli.entry.addr = addr;
                    cli.entry.sock = ev.data.fd;
                    cli.hasSentUsername = false;
                    cli.isPlayerReady = false;
                    
                    clientList.insert({getPlayerId(), cli});

                    if (epoll_ctl(epollfd, EPOLL_CTL_ADD, ev.data.fd, &ev) < 0) {
                        perror("Epoll_ctl");
                        close(ev.data.fd);
                        continue;
                    }
                } else {
                    logv("Reading data\n");
                    while ((nbytes = recv(events[i].data.fd, buff, USHRT_MAX - 1, 0)) > 0) {
                        //Handle message
                        if (nbytes < 4) {
                            perror("Packet read was too small");
                            continue;
                        }
                        buff[nbytes] = '\0';
                        processTCPMessage(buff, nbytes, events[i].data.fd);
                    }
                    if (errno == EAGAIN || errno == EWOULDBLOCK) {
                        logv("Safe error\n");
                        continue;
                    }
                    perror("Packet read failure");
                    close(events[i].data.fd);
                }
            }
        }
    }
    free(events);
}

int32_t getPlayerId() {
    static std::atomic<int32_t> counter{0};
    return ++counter;
}

void alarmHandler(int signo) {
    startTimer();
    genOutputPacket();
    sendSyncPacket(sendSocketUDP);
    clearMoveActions();
}

void listenForPackets(const struct sockaddr_in servaddr) {
    int epollfd;
    epoll_event ev;
    epoll_event *events;
    socklen_t servAddrLen = sizeof(servaddr);

    if (!(events = (epoll_event *) calloc(MAXEVENTS, sizeof(epoll_event)))) {
        perror("Calloc failure");
        exit(1);
    }

    if ((epollfd = epoll_create1(0)) == -1) {
        perror("Epoll create");
        exit(1);
    }

    ev.events = EPOLLIN | EPOLLET | EPOLLEXCLUSIVE;
    ev.data.fd = listenSocketUDP;

    if ((epoll_ctl(epollfd, EPOLL_CTL_ADD, listenSocketUDP, &ev)) == -1) {
        perror("epoll_ctl");
        exit(1);
    }

    char buff[IN_PACKET_SIZE];
    ssize_t nbytes = 0;
    int nevents = 0;
    for (;;) {
        if ((epoll_wait(epollfd, &ev, 1, -1)) == -1) {
            perror("epoll_wait");
            exit(1);
        }
#pragma omp parallel for schedule (static)
        for (int i = 0; i < nevents; ++i) {
            if (events[i].events & EPOLLERR) {
                perror("Socket error");
                close(events[i].data.fd);
                continue;
            }
            if (events[i].events & EPOLLHUP) {
                //Peer closed the connection
                close(events[i].data.fd);
                continue;
            }
            if (events[i].events & EPOLLIN) {
                while ((nbytes = recvfrom(listenSocketUDP, buff, IN_PACKET_SIZE, 
                        0, (sockaddr *) &servaddr, &servAddrLen)) > 0) {
#pragma omp task
                    processPacket(buff);
                }
            }
        }
    }
}

void processPacket(const char *data) {
    //Actual implementation TBD
    const MoveAction *ma = reinterpret_cast<const MoveAction *>(data);
    updateMarine(*ma);
    saveMoveAction(ma->id, *ma);
}

//Isaac Morneau Feb 28th, 2017
void genOutputPacket() {
    int32_t *pBuff = (int32_t*)outputPacket;
    //get all the players
    const auto& players = getPlayers();
    //get all the zombies
    const auto& zombies = getZombies();
    
    //start of every sync is the packet header
    *pBuff++ = SYNC;
    //construct the sub header for players
    *pBuff++ = PLAYERH;
    *pBuff++ = players.size();
    PlayerData* pPlayer = reinterpret_cast<PlayerData*>(pBuff);
    //write all the players to the buffer
    for(auto p : players) {
        p.nmoves = 0;
        p.nattacks = 0;
        memcpy(pPlayer++, &p, sizeof(PlayerData));
    }
    pBuff = reinterpret_cast<int32_t*>(pPlayer);
    //construct the sub header for zombies
    *pBuff++ = ZOMBIEH;
    *pBuff++ = zombies.size();
    ZombieData* pZombie = reinterpret_cast<ZombieData*>(pBuff);
    //write all the zombies to the buffer
    for(auto z : zombies) {
        memcpy(pZombie++, &z,sizeof(ZombieData));
    }
    pBuff = reinterpret_cast<int32_t*>(pZombie);
    //calculate how full the packet is for when its sent
    outputLength = pBuff - (int32_t*)outputPacket;
}

void sendSyncPacket(int sock) {
    for (const auto& client : clientList) {
        sendto(client.second.entry.sock, outputPacket, outputLength, 0, (const sockaddr *) &client.second.entry.addr, sizeof(client.second.entry.addr));
    }
}

void startTimer() {
    signal(SIGALRM, &alarmHandler);
    struct itimerval duration;
    memset(&duration, 0, sizeof(duration));
    duration.it_interval.tv_usec = microSecPerTick;
    setitimer(ITIMER_REAL, &duration, NULL);
    logv("Timer started\n");
}

void listenTCP(int socket, unsigned long ip, unsigned short port) {
    //TCP Setup
    struct sockaddr_in servaddrtcp;
    memset(&servaddrtcp, 0, sizeof(servaddrtcp));
    servaddrtcp.sin_family = AF_INET;
    servaddrtcp.sin_addr.s_addr = htonl(ip);
    servaddrtcp.sin_port = htons(port); 

    if ((bind(socket, (struct sockaddr *) &servaddrtcp, sizeof(servaddrtcp))) == -1) {
        perror("Bind TCP");
        exit(1);
    }

    if (listen(socket, LISTENQ) == -1) {
        perror("Listen TCP");
        exit(1);
    }
}

void listenUDP(int socket, unsigned long ip, unsigned short port) {
    //UDP Setup
    struct sockaddr_in servaddrudp;
    memset(&servaddrudp, 0, sizeof(servaddrudp));
    servaddrudp.sin_family = AF_INET;
    servaddrudp.sin_addr.s_addr = htonl(ip);
    servaddrudp.sin_port = htons(port); 

    if ((bind(socket, (struct sockaddr *) &servaddrudp, sizeof(servaddrudp))) == -1) {
        perror("Bind UDP");
        exit(1);
    }
    startTimer();
    logv("UDP server started\n");
    listenForPackets(servaddrudp);
}

int createSocket(bool useUDP, bool nonblocking) {
    return socket(AF_INET, ((useUDP) ? SOCK_DGRAM : SOCK_STREAM) | (nonblocking * SOCK_NONBLOCK), 0);
}

void logv(const char *msg, ...) {
    if(!verbose) {
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
    if ((outBuff = (char *) malloc(mesgSize + 6)) == nullptr) {
        perror("Malloc failure");
        exit(1);
    }
    memset(outBuff, '\0', mesgSize + 6);
    reinterpret_cast<int32_t *>(outBuff)[0] = id;
    outBuff[4] = (isConnectMessage) ? 'C' : 'T';
    outBuff[5] = '/';

    strncpy(outBuff + 6, mesg, mesgSize);

    for (const auto& clients : clientList) {
        if (clients.first != id && clients.second.hasSentUsername) {
            if (send(clients.second.entry.sock, outBuff, mesgSize + 6, 0) < 0) {
                perror("Failed to send client messasge");
            }
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
void processTCPMessage(const char *buff, const size_t nbytes, int sock) {
    //Convert first 4 chars to 32 bit int representing id
    const int32_t idReceived = reinterpret_cast<const int32_t *>(buff)[0];

    logv("Read packet with id: %d\n", idReceived);

    for (const auto& it : clientList) {
        if (it.second.entry.sock == sock) {
            if (!it.second.hasSentUsername) {
                //Handle initial username read
                std::pair<int32_t, PlayerJoin> tempMapEntry;
                tempMapEntry = it;
                tempMapEntry.first = getPlayerId();
                tempMapEntry.second.hasSentUsername = true;
                tempMapEntry.second.isPlayerReady = false;
                strncpy(tempMapEntry.second.entry.username, buff + 6, NAMELEN);
                strcat(tempMapEntry.second.entry.username, "\0");
                logv("Server received username: %s\n", tempMapEntry.second.entry.username);

                //Erase temporary entry in client list
                clientList.erase(clientList.find(it.first));
                //Insert newly compelted entry
                clientList.insert(tempMapEntry);

                const size_t bufferSize = NAMELEN + 4 + 2;
                char outBuff[bufferSize];
                memset(outBuff, '\0', bufferSize);
                int32_t *id = reinterpret_cast<int32_t *>(outBuff);
                *id = tempMapEntry.first;
                outBuff[4] = 'C';
                outBuff[5] = '/';

                strncpy(outBuff + 6, tempMapEntry.second.entry.username, NAMELEN);
                //Send client their allocated id and username
                if (send(sock, outBuff, bufferSize, 0) < 0) {
                    perror("Failed to send client message");
                    break;
                }

                //Send new client a list of already existing clients 
                for (const auto& it : clientList) {
                    if (it.second.entry.sock != sock && it.second.hasSentUsername) {
                        memset(outBuff, '\0', bufferSize);
                        id = reinterpret_cast<int32_t *>(outBuff);
                        *id = it.first;
                        outBuff[4] = 'C';
                        outBuff[5] = '/';
                        strncpy(outBuff + 6, tempMapEntry.second.entry.username, NAMELEN);

                        if (send(sock, outBuff, bufferSize, 0) < 0) {
                            perror("Failed to send client message");
                            break;
                        }
                    }
                }
            } else {
                if (buff[5] == '/') {
                    //Command message
                    if (strncmp(buff + 6, "ready", nbytes - 6) == 0) {
                        //Ready command
                        if (clientList.count(idReceived)) {
                            clientList[idReceived].isPlayerReady = true;
                            logv("Player id %d is ready to start\n", idReceived);
                            sendTCPClientMessage(idReceived, true, "ready", 5);
                        } else {
                            perror("Client not found in list");
                        }
                        bool areAllReady = true;
                        for (const auto& elem : clientList) {
                            if (!elem.second.isPlayerReady) {
                                areAllReady = false;
                                break;
                            }
                        }
                        if (areAllReady) {
                            transitionToGameStart();
                        }
                    } else if (strncmp(buff + 6, "unready", nbytes - 6) == 0) {
                        //Unready command
                        if (clientList.count(idReceived)) {
                            clientList[idReceived].isPlayerReady = false;
                            logv("Player id %d is NOT ready to start\n", idReceived);
                            sendTCPClientMessage(idReceived, true, "unready", 7);
                        } else {
                            perror("Client not found in list");
                        }
                    } else if (strncmp(buff + 6, "start", nbytes - 6) == 0) {
                        //Start game command
                        transitionToGameStart();
                    }
                } else {
                    //Regular chat message - processing TBD
                    logv("Server received: %s\n", buff + 4);
                }
            }
            break;
        }
    }
}
