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
#include "server.h"

int main(int argc, char **argv) {
    int opt;
#pragma omp parallel
    while ((opt = getopt(argc, argv, OPT_STRING)) != -1) {
        switch(opt) {
            case 'a':
                break;
            case 'b':
                break;
            case 'c':
                break;
            case 'd':
                break;
        }
    }

    if ((listenSocket = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
        perror("ListenSocket");
        exit(1);
    }
    if ((sendSocket = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
        perror("sendSocket");
        exit(1);
    }

    if ((fcntl(listenSocket, F_SETFL, O_NONBLOCK)) == -1) {
        perror("ListenSocket fcntl");
        exit(1);
    }
    if ((fcntl(sendSocket, F_SETFL, O_NONBLOCK)) == -1) {
        perror("sendSocket fcntl");
        exit(1);
    }
    struct sockaddr_in servaddr;
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(LISTEN_PORT); 

    if ((bind(listenSocket, (struct sockaddr *) &servaddr, sizeof(servaddr))) == -1) {
        perror("Listen bind");
        exit(1);
    }

    struct itimerval duration;
    memset(&duration, 0, sizeof(duration));
    duration.it_interval.tv_usec = microSecPerTick;

    setitimer(ITIMER_REAL, &duration, NULL);

    listenForPackets(servaddr);

    return 0;
}

void alarmHandler(int signo) {
    struct itimerval duration;
    memset(&duration, 0, sizeof(duration));
    duration.it_interval.tv_usec = microSecPerTick;
    setitimer(ITIMER_REAL, &duration, NULL);
    genOutputPacket();
    sendSyncPacket(sendSocket);
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
    ev.data.fd = listenSocket;

    if ((epoll_ctl(epollfd, EPOLL_CTL_ADD, listenSocket, &ev)) == -1) {
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
        ssize_t read = recvfrom(listenSocket, buff, IN_PACKET_SIZE, 0, (struct sockaddr *) &servaddr, &servAddrLen);
        
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
    for (size_t i = 0; i < CLIENT_COUNT; ++i) {
        sendto(sock, outputPacket, OUT_PACKET_SIZE, 0, (const struct sockaddr *) &clientAddrs[i], sizeof(clientAddrs[i]));
    }
}
