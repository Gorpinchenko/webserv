#include "Socket.hpp"


Socket::Socket(in_addr_t ip, uint16_t port): ip(ip), port(port) {
    (void) this->ip;
    (void) this->port;
}

int Socket::getSocketFd() const {
    return socket_fd;
}
