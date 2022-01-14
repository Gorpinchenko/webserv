#ifndef WEBSERV_SOCKET_HPP
#define WEBSERV_SOCKET_HPP

#include <iostream>

class Socket {
private:
    in_addr_t ip;
    uint16_t port;
    int socket_fd;
//    VirtualServer *_default_config;
//    std::map<int, Session*> _sessions;
//    std::map<std::string, VirtualServer> _virtual_servers;
//    Server *_serv;
public:
    Socket(in_addr_t ip, uint16_t port);
    int getSocketFd() const;
};


#endif //WEBSERV_SOCKET_HPP
