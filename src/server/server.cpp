#include <omp.h>
#include <unistd.h>
#include <sys/epoll.h>
#include <sys/signal.h>

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
int sendSocketUDP;
bool verbose = false;
std::unordered_map<int32_t, PlayerJoin> clientList;
std::atomic_bool isGameRunning{false};

/**
 * Main entry function
 * Isaac Morneau March 19
 */
int main(int argc, char **argv) {
    setenv("OMP_PROC_BIND", "TRUE", 1);
    setenv("OMP_DYNAMIC", "TRUE", 1);
    signal(SIGINT, SIG_IGN);

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
            case 'v'://verbose
                log_verbose = 2;
                break;
            case 'e'://error
                log_verbose = 1;
                break;
            case 'o':
                log_verbose = atoi(optarg);
                break;
            case '?':
                printf("-v verbose\n-e error\nverbose enables error as well.");
                break;
        }
    }
    if (listen_port_udp == listen_port_tcp){
        printf("l cannot be the same port as L\n");
        exit(2);
    }

    logv("UDP port: %d, TCP port: %d, Max clients %d\n", listen_port_udp, listen_port_tcp, client_count);

    listenSocketUDP = createSocket(true, true);
    sendSocketUDP = createSocket(true, true);
    listenSocketTCP = createSocket(false, true);

    listenTCP(listenSocketTCP, INADDR_ANY, listen_port_tcp);
    logv("Sockets created and bound\n");
    initSync(listenSocketTCP);
    return 0;
}

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
    for (const auto& client : clientList) {
        sendto(sock, outputPacket, outputLength, 0, (const sockaddr *) &client.second.entry.addr, sizeof(client.second.entry.addr));
    }
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

