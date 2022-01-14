#include "Socket.hpp"


Socket::Socket(in_addr_t ip, uint16_t port) : ip(ip), port(port) {
    int         fd;
    int         opt  = 1;
    sockaddr_in addr = {};

    fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (fd == -1) {
        throw SocketException(std::strerror(errno));
    }

    this->socket_fd = fd;

    memset(addr.sin_zero, 0, 8);
    addr.sin_len         = sizeof(addr);
    addr.sin_family      = AF_INET;
    addr.sin_port        = htons(this->port);
    addr.sin_addr.s_addr = this->ip;

    if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(int)) == -1
        || fcntl(fd, F_SETFL, O_NONBLOCK) == -1
        || bind(fd, (struct sockaddr *) &addr, sizeof(addr)) == -1
        || listen(fd, MAX_AWAIT_CONN) == -1) {
        close(fd);
        throw SocketException(std::strerror(errno));
    }
}

in_addr_t Socket::getIp() const {
    return this->ip;
}

uint16_t Socket::getPort() const {
    return this->port;
}

int Socket::getSocketFd() const {
    return this->socket_fd;
}

Socket::SocketException::SocketException(const char *msg) : m_msg(msg) {}

Socket::SocketException::~SocketException() throw() {}

const char *Socket::SocketException::what() const throw() {
    std::cerr << "SocketError: ";
    return this->m_msg;
}
