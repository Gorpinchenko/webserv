#ifndef WEBSERV_SOCKET_HPP
#define WEBSERV_SOCKET_HPP

#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <arpa/inet.h>

#define MAX_AWAIT_CONN 900

class Socket {
private:
    in_addr_t ip;
    uint16_t  port;
    int       socket_fd;
//    VirtualServer *_default_config;
//    std::map<int, Session*> _sessions;
//    std::map<std::string, VirtualServer> _virtual_servers;
//    Server *_serv;
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
