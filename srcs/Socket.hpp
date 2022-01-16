#ifndef WEBSERV_SOCKET_HPP
#define WEBSERV_SOCKET_HPP

#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include "IEventSubscriber.hpp"

#define MAX_AWAIT_CONN 900

class Socket : public IEventSubscriber {
private:
    in_addr_t ip;
    uint16_t  port;
    int       socket_fd;
public:
    Socket(in_addr_t ip, uint16_t port);
    in_addr_t getIp() const;
    uint16_t getPort() const;
    int getSocketFd() const;

    class SocketException : public std::exception {
        const char *m_msg;
    public:
        explicit SocketException(const char *msg);

        ~SocketException() throw();

        const char *what() const throw();
    };
};


#endif //WEBSERV_SOCKET_HPP
