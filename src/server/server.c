#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
#include <stdarg.h>
#include <stdbool.h>

#include "server.h"

//declared here so they can be overriden with flags at run time as needed
int listen_port_udp = LISTEN_PORT_UDP;
int listen_port_tcp = LISTEN_PORT_TCP;
int client_count = CLIENT_COUNT;

int main(int argc, char **argv) {
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
    if ((sendSocketTCP = createSocket(false, true)) == -1) {
        perror("sendSocket TCP");
        exit(1);
    }

    listenTCP(listenSocketTCP, INADDR_ANY, listen_port_tcp);

    logv("Sockets created and bound\n");

    initSync(listenSocketTCP);

    listenUDP(listenSocketUDP, INADDR_ANY, listen_port_udp);
    return 0;
}

void initSync(int sock){
    logv("Starting TCP sync\n");
    int clientFDs[client_count];
    socklen_t clientLens[client_count];
    //make this also hold player data
    clients = (Client *) calloc(client_count, sizeof(Client));
    //accept all the connections
    //get the basic info from each
    for(int i = 0; i < client_count; ++i) {
        clients[i].addr = (struct sockaddr_in*) calloc(1, sizeof(struct sockaddr_in));
        clients[i].player = (Player*) calloc(1, sizeof(Player));

        clientLens[i] = sizeof(struct sockaddr_in);
        clientFDs[i] = accept(sock, (struct sockaddr *) clients[i].addr, &clientLens[i]);

        char inbuff[SYNC_IN+1], outbuff[SYNC_OUT];
        memset(outbuff, 0, SYNC_OUT);

        int r;
        if ((r = read(clientFDs[i], inbuff, SYNC_IN)) > 0){
            inbuff[r] = '\0';
            logv("connected:%s with id:%d\n", inbuff, i);
            strcpy(clients[i].player->username,inbuff);
            clients[i].player->id = i;
            strcpy(outbuff, inbuff);
            outbuff[SYNC_OUT-1] = i;
            
            //tell the client who they are
            if (write(clientFDs[i], outbuff, SYNC_OUT) == -1) {
                perror("failed to write to client user info");
                exit(4);
            }

            //update others of the clients addition
            for (int j = 0; j < i; ++j){
                if (write(clientFDs[j], outbuff, SYNC_OUT) == -1) {
                    perror("failed to write to client updated user list");
                    exit(4);
                }
            }

            //tell new clients previously connected clients
            for (int j = 0; j < i; ++j) {
                strcpy(outbuff, clients[j].player->username);
                outbuff[SYNC_OUT-1] = clients[j].player->id;
                if (write(clientFDs[i], outbuff, SYNC_OUT) == -1) {
                    perror("failed to write to new client list of other users");
                    exit(4);
                }
            }

            logv("updated %d clients\n", i);
        } else if (r == 0){//file closed
            break;
        } else if (r == -1) {
            perror("Packet read failure");
            exit(1);
        }
    }
    //close all the connections
#if 0
    while(i){
        logv("Closing TCP client %d\n",i);
        write(clientFDs[i],"start",6);
        close(clientFDs[i--]);
    }
#endif
    logv("TCP sync complete\n");
}

void alarmHandler(int signo) {
    startTimer();
    genOutputPacket();
    sendSyncPacket(sendSocketUDP);
}

void listenForPackets(const struct sockaddr_in servaddr) {
    int epollfd;
    struct epoll_event ev;
    socklen_t servAddrLen = sizeof(servaddr);

    if ((epollfd = epoll_create1(0)) == -1) {
        perror("Epoll create");
        exit(1);
    }

    ev.events = EPOLLIN | EPOLLET;
    ev.data.fd = listenSocketUDP;

    if ((epoll_ctl(epollfd, EPOLL_CTL_ADD, listenSocketUDP, &ev)) == -1) {
        perror("epoll_ctl");
        exit(1);
    }

    char buff[IN_PACKET_SIZE];
#pragma omp parallel shared(epollfd, ev) private(buff) 
    for (;;) {
        if ((epoll_wait(epollfd, &ev, 1, -1)) == -1) {
            perror("epoll_wait");
            exit(1);
        }
        ssize_t read = recvfrom(listenSocketUDP,
                buff,
                IN_PACKET_SIZE,
                0,
                (struct sockaddr *) &servaddr,
                &servAddrLen);

        if (read == -1) {
            if (errno == EAGAIN || errno == EWOULDBLOCK) {
                continue;
            }
            perror("Packet read failure");
            exit(1);
        }
        if (read == IN_PACKET_SIZE) {
#pragma omp task
            processPacket(buff);
        }
    }
}

void processPacket(const char *data) {
    //Actual implementation TBD
}

void genOutputPacket() {
    //Actual implementation TBD
    strcpy(outputPacket, "");
}

void sendSyncPacket(int sock) {
#pragma omp parallel for
    for (size_t i = 0; i < client_count; ++i) {
        sendto(sock,
                outputPacket,
                OUT_PACKET_SIZE,
                0,
                (const struct sockaddr *) clients[i].addr,
                sizeof(*clients[i].addr));
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
