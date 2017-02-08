#ifndef UDPCLIENT_HPP
#define UDPCLIENT_HPP

#include <sys/socket.h>
#include <arpa/inet.h>

class UDPSocket {
public:
    UDPSocket();
    ~UDPSocket();
    void bindTo(int);
    void sendTo(const char *, int, struct sockaddr_in);
    void receiveFrom(char *, int, struct sockaddr_in);
private:
    int _sockUDP;
};

#endif
