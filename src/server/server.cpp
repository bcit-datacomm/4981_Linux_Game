#include <omp.h>
#include <unistd.h>
#include <sys/epoll.h>
#include <sys/signal.h>
#include <arpa/inet.h>

#include "../UDPHeaders.h"
#include "../log/log.h"
#include "server.h"
#include "serverwrappers.h"
#include "servergamestate.h"

//declared here so they can be overriden with flags at run time as needed
int listen_port_udp = LISTEN_PORT_UDP;
int listen_port_tcp = LISTEN_PORT_TCP;
size_t client_count = CLIENT_COUNT;
int outputLength = 0;

char outputPacket[OUT_PACKET_SIZE];
int listenSocketUDP;
int listenSocketTCP;
sockaddr_in sendAddrUDP;
int sendSocketUDP;
bool verbose = false;
std::unordered_map<int32_t, PlayerJoin> clientList;
std::atomic_bool isGameRunning{false};

/**
 * The TCP sync loop.
 * It takes the socket to listen on as a parameter.
 * It uses epoll to wait for a read notification, upon which it dispatches control
 * to the appropriate handler.
 * John Agapeyev March 19
 */
void initSync(const int sock) {
    logv("Starting TCP sync\n");

    epoll_event *events = createEpollEventList();

    epoll_event ev;
    ev.events = EPOLLIN | EPOLLET | EPOLLEXCLUSIVE;
    ev.data.fd = sock;

    int epollfd = createEpollFD();
    addEpollSocket(epollfd, ev.data.fd, &ev);

    int nevents = 0;
    for (;;) {
        nevents = waitForEpollEvent(epollfd, events);
        for (int i = 0; i < nevents; ++i) {
            if (events[i].events & EPOLLERR) {
                perror("Socket error");
                for (const auto& it : clientList) {
                    if (it.second.entry.sock == events[i].data.fd) {
                        clientList.erase(it.first);
                        break;
                    }
                }
                close(events[i].data.fd);
                continue;
            }
            if (events[i].events & EPOLLHUP) {
                //Peer closed the connection
                logv("Peer closed connection\n");
                for (const auto& it : clientList) {
                    if (it.second.entry.sock == events[i].data.fd) {
                        clientList.erase(it.first);
                        break;
                    }
                }
                close(events[i].data.fd);
                continue;
            }
            if (events[i].events & EPOLLIN) {
                if (events[i].data.fd == sock) {
                    handleIncomingTCP(epollfd);
                } else {
                    readTCP(events[i].data.fd);
                }
            }
        }
    }
    free(events);
}

/**
 * The main server loop during gameplay.
 * This is the UDP loop, as opposed to the TCP loop defined above.
 * This method takes the address to use when reading from the socket.
 * John Agapeyev March 19
 */
void listenForPackets(const sockaddr_in servaddr) {
    socklen_t servAddrLen = sizeof(servaddr);

    epoll_event *events = createEpollEventList();

    epoll_event ev;
    ev.events = EPOLLIN | EPOLLET | EPOLLEXCLUSIVE;
    ev.data.fd = listenSocketUDP;

    int epollfd = createEpollFD();
    addEpollSocket(epollfd, listenSocketUDP, &ev);

    int nevents = 0;
    for (;;) {
        nevents = waitForEpollEvent(epollfd, events);
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
#pragma omp task
                readUDP(events[i].data.fd, (sockaddr *) &servaddr, &servAddrLen);
            }
        }
    }
}

/**
 * Processes a char buffer that was received as a UDP packet.
 * It casts the buffer to a struct defined in UDPHeaders.h.
 * It then uses a predefined enum id to determine the packet type.
 * This packet type is then handled appropriately.
 * John Agapeyev March 19
 */
void processPacket(const char *data) {
    const ClientMessage *mesg = reinterpret_cast<const ClientMessage *>(data);
    switch (static_cast<UDPHeaders>(mesg->id)) {
        case UDPHeaders::WALK:
            {
                const MoveAction& ma = mesg->data.ma;
                logv("Move actions packets contents:\nID:%d\nXpos:%f\n, Ypos:%f\n, Vel:%f\n, Direction:%f\n", ma.id, ma.xpos, ma.ypos, ma.vel, ma.direction);
                updateMarine(ma);
            }
            break;
        case UDPHeaders::ATTACKACTIONH:
            {
                const AttackAction& aa = mesg->data.aa;
                saveAttack(aa);
            }
            break;
        default:
            logv("Received packet with unknown id\n");
            break;
    }
}

/**
 * Generates the contents of the output packet and stores them into a buffer.
 * This method takes advantage of reinterpret casting pointers to write multi-byte
 * values into char arrays.
 * At the end, it then calculates the size of the generated packet.
 * This size is required due to the dynamic size of the packet, and as such, the packet
 * size is not always predefined.
 * Isaac Morneau Feb 28th, 2017
 */
void genOutputPacket() {
    int32_t *pBuff = (int32_t *) outputPacket;

    //start of every sync is the packet header
    *pBuff++ = static_cast<int32_t>(UDPHeaders::SYNCH);

    //construct the sub header for players
    *pBuff++ = static_cast<int32_t>(UDPHeaders::PLAYERH);
    //get all the players
    const auto& players = getPlayers();
    *pBuff++ = players.size();
    PlayerData *pPlayer = reinterpret_cast<PlayerData *>(pBuff);
    //write all the players to the buffer
    for(const auto& p : players) {
        memcpy(pPlayer++, &p, sizeof(PlayerData));
    }
    pBuff = reinterpret_cast<int32_t *>(pPlayer);

    *pBuff++ = static_cast<int32_t>(UDPHeaders::ATTACKACTIONH);
    *pBuff++ = attackList.size();
    AttackAction *pAttack = reinterpret_cast<AttackAction *>(pBuff);
    for (const auto& aa : attackList) {
        memcpy(pAttack++, &aa, sizeof(AttackAction)); 
    }
    pBuff = reinterpret_cast<int32_t *>(pAttack);

    //construct the sub header for zombies
    *pBuff++ = static_cast<int32_t>(UDPHeaders::ZOMBIEH);
    //get all the zombies
    const auto& zombies = getZombies();
    *pBuff++ = zombies.size();
    ZombieData* pZombie = reinterpret_cast<ZombieData *>(pBuff);
    //write all the zombies to the buffer
    for(const auto& z : zombies) {
        memcpy(pZombie++, &z, sizeof(ZombieData));
    }
    pBuff = reinterpret_cast<int32_t *>(pZombie);

    //calculate how full the packet is for when its sent
    outputLength = (pBuff - (int32_t *) outputPacket) * sizeof(int32_t);
}

/**
 * Loops through every client and sends them a copy of the output sync packet.
 * John Agapeyev March 19
 */
void sendSyncPacket(const int sock) {
    sendto(sock, outputPacket, outputLength, 0, (const sockaddr *) &sendAddrUDP, sizeof(sendAddrUDP));
}

/**
 * Sets a TCP socket to listen on a given ip and port.
 * The ip and port are in network byte order.
 * John Agapeyev March 19
 */
void listenTCP(const int socket, const unsigned long ip, const unsigned short port) {
    bindSocket(socket, ip, port);
    if (listen(socket, LISTENQ) == -1) {
        perror("Listen TCP");
        exit(1);
    }
}

/**
 * Sets a UDP socket to listen on a given ip and port
 * The only difference between this method and the TCP method defined above is that
 * the TCP method calls listen and bind, whereas this method only calls bind.
 * Another key difference is that this method automatically starts the UDP listen loop.
 * John Agapeyev March 19
 */
void listenUDP(const int socket, const unsigned long ip, const unsigned short port) {
    const auto& servaddrudp = bindSocket(socket, ip, port);
    logv("UDP server started\n");
    listenForPackets(servaddrudp);
}

