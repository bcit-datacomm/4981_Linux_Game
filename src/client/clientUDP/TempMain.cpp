#include "UDPSocket.hpp"
#include <thread>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include "SyncHandling.hpp"

#define UDP_PORT 35522
#define SERVER_IP "192.168.0.7"

int main() {
    const char *ip = SERVER_IP;
    int port = UDP_PORT;
    init(ip, port);
}
