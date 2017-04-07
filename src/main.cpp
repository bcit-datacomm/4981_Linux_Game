/*------------------------------------------------------------------------------
* Source: main.cpp     
*
* Functions:
*               main
*
* Date:     Jan. 20, 2017
*
* Revisions: 
* Edited By : Yiaoping Shu- Style guide
*
*
* Designer: Isaac Morneau + Jacob McPhail 
*
* Author:   Isaac Morneau + Jacob McPhail 
*
*       
------------------------------------------------------------------------------*/
#include <cstdio>
#include <iostream>
#include <string>
#include <getopt.h>
#include <sys/signal.h>
#include <arpa/inet.h>

#include "server/server.h"
#include "server/serverwrappers.h"
#include "client/NetworkManager.h"
#include "game/Game.h"
#include "log/log.h"

/**
* Date: Jan. 20, 2017
* Author: Jacob McPhail
* Function Interface: int main(int argc, char **argv)
* Description: 
*   Processes command args, and starts the game.
*/
int main(int argc, char **argv) {
    setenv("OMP_PROC_BIND", "TRUE", 1);
    setenv("OMP_DYNAMIC", "TRUE", 1);
    setenv("OMP_NESTED", "TRUE", 1);

    int opt;
    while ((opt = getopt(argc, argv, OPT_STRING.c_str())) != -1) {
        switch(opt) {
            case 'h':
                printf("usage %s\noptions:\n\t"
                        " -h this message\n\t"
#ifdef SERVER
                        "-l the port to listen to for UDP, default 35222\n\t"
                        "-L the port to listen to for TCP, default 35223\n\t"
                        "-c the number of clients to accept max, default 10\n"
#endif
                        "-v verbose\n-e error\nverbose enables error as well.",
                        argv[0]);
                exit(0);
                break;
#ifdef SERVER
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
#endif
            case 'n':
                networked = true;
                break;
            case 'i'://verbose
                log_verbose = 3;
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
                printf("-v verbose\n-e error\nverbose enables error as well.\n");
                break;
        }
    }
#ifdef SERVER
    if (listen_port_udp == listen_port_tcp){
        printf("l cannot be the same port as L\n");
        exit(2);
    }

    logv("UDP port: %d, TCP port: %d, Max clients %d\n", listen_port_udp, listen_port_tcp, client_count);

    listenSocketUDP = createSocket(true, true);
    sendSocketUDP = createSocket(true, true);
    listenSocketTCP = createSocket(false, true);

    fillMulticastAddr(sendAddrUDP);

    memset(&udpMesgs, 0, sizeof(udpMesgs));

#pragma omp parallel for schedule(static)
    for (int i = 0; i < MAX_UDP_PACKET_COUNT; ++i) {
        iovecs[i].iov_base = readBuffers[i];
        iovecs[i].iov_len = IN_PACKET_SIZE;
        udpMesgs[i].msg_hdr.msg_iov = &iovecs[i];
        udpMesgs[i].msg_hdr.msg_iovlen = 1;
    }

    listenTCP(listenSocketTCP, INADDR_ANY, listen_port_tcp);
    logv("Sockets created and bound\n");
    initSync(listenSocketTCP);
#else 
    Game game;

    logv("Loading...\n");

    //Start up SDL and create window
    if (game.init() && game.loadMedia()) {
        logv("Running...\n");
        game.run();
    } else {
        logv("Failed to start!\n");
    }
    logv("Exit\n");
#endif
    return 0;
}

