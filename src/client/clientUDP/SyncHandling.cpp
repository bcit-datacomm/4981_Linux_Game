#include "SyncHandling.hpp"
#include <thread>
#include <iostream>
#include <string.h>
#include <unistd.h>

#define TOTAL_TESTS 4
const char *testData[TOTAL_TESTS] = {"defaultusername", "aname",
    "brodyMcCrone", "testytest"};
int testIndex = 0;

void init(const char *servIP, int stdPort) {
    UDPSocket sockUDP;
    sockUDP.bindTo(stdPort);

    struct sockaddr_in servAddr;
    memset((char *) &servAddr, 0, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_port = htons(stdPort);
    servAddr.sin_addr.s_addr = inet_addr(servIP);

    std::thread sendThread(sendloop, &sockUDP, servAddr);
    sendThread.detach();

    std::thread recvThread(recvloop, &sockUDP, servAddr);
    recvThread.join();
}

void sendloop(UDPSocket *sendSockUDP, struct sockaddr_in servAddr) {
    while(true) {
        const char *data = retrieve();
        Packet pkt = packetize(data);

        std::cout << "Sending: " << pkt.buf << std::endl;
        sendSockUDP->sendTo(pkt.buf, pkt.len, servAddr);
        usleep(1000 * 100);
    }
}

const char *retrieve() {
    //TEMPORARY IMPLEMENTATION
    //NEED TO KNOW HOW DATA WILL BE PASSED FROM GAME THREAD TO NETWORK THREAD
    if(++testIndex == TOTAL_TESTS)
        testIndex = 0;
    return testData[testIndex];
}

Packet packetize(const char *data) {
    //TEMPORARY IMPLEMENTATION
    Packet pkt;
    strncpy(pkt.buf, data, BUFFER_SIZE);
    pkt.len = strlen(data);
    return pkt;
}

void recvloop(UDPSocket *recvSockUDP, struct sockaddr_in servAddr) {
    char buf[BUFFER_SIZE];
    while(true) {
        recvSockUDP->receiveFrom(buf, BUFFER_SIZE, servAddr);
        std::cout << "Received: " << buf << std::endl;
        char *data = depacketize(buf);
        process(data);
    }
}

char *depacketize(char *packetizedData) {
    //TO BE IMPLEMENTED
    return packetizedData;
}

void process(char *) {
    //TO BE IMPLEMENTED
    //PRESUMABLY MAKE JACOBS API CALL HERE
}
