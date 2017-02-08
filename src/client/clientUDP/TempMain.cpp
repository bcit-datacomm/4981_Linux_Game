#include "UDPSocket.hpp"
#include <thread>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include "SyncHandling.hpp"

#define UDP_PORT 35522
#define SERVER_IP "127.0.0.1"

int main() {
    const char *ip = SERVER_IP;
    int port = UDP_PORT;
    init(ip, port);
}
