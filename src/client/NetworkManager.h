#ifndef NETWORKMANAGER_HPP
#define NETWORKMANAGER_HPP

#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <climits>
#include <atomic>

#include "../UDPHeaders.h"
#include "../server/server.h"

static constexpr int STDIN = 0;
static constexpr int STD_BUFFSIZE = 1024;
static constexpr int MAX_EVENTS = 1;
static constexpr int MAX_USERS = 23;
static constexpr int UNAME_SIZE = 32;
static constexpr int SYNC_PACKET_MAX = USHRT_MAX;

extern bool networked;

//order matters.
enum class NetworkState {
    NOT_RUNNING,
    INITIALIZING,
    FAILED_TO_CONNECT,
    CONNECTED,
    GAME_STARTED
};

class NetworkManager {
public:
    static NetworkManager& instance();
    ~NetworkManager();

    void run(const std::string ip, const std::string username);
    void writeUDPSocket(const char *buf, const int len) const;
    int32_t getPlayerId() const {return myid;};
    NetworkState getNetworkState() const {return state;};
    void reset() {state = NetworkState::NOT_RUNNING;};
private:
    NetworkManager() = default;

    void runUDPClient(const in_addr_t serverIP);
    void initTCPClient(const in_addr_t serverIP, const std::string username);
    void runTCPClient(const std::string username);
    void handshake(const std::string username) const;
    void waitRecvId();
    void writeTCPSocket(const char *buf, const int len) const;
    int readTCPSocket(char *buf, const int len) const;
    int readUDPSocket(char *buf, const int len) const;

    static sockaddr_in createAddress(const in_addr_t ip, const int port);
    static bool connectSocket(const int sock, const sockaddr_in& addr);

    std::atomic<NetworkState> state{NetworkState::NOT_RUNNING};
    int32_t myid;  // EY: March 14 - to be removed for game intergration
    int sockTCP;
    int sockUDP;
    sockaddr_in servUDPAddr;
    socklen_t servUDPAddrLen;
};

#endif
