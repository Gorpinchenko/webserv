#ifndef WEBSERV_SOCKET_HPP
#define WEBSERV_SOCKET_HPP

#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include "IEventSubscriber.hpp"
#include "Server.hpp"

#define MAX_AWAIT_CONN 900

class Socket : public IEventSubscriber {
private:
    Server *server;
    int    fd;
public:
    Socket(Server *server);
    int getFd() const;
    Server *getServer() const;

    class SocketException : public std::exception {
        const char *m_msg;
    public:
        explicit SocketException(const char *msg);

        ~SocketException() throw();

        const char *what() const throw();
    };
};


#endif //WEBSERV_SOCKET_HPP
