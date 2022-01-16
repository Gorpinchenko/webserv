#ifndef WEBSERV_DAEMON_HPP
#define WEBSERV_DAEMON_HPP

#include <iostream>
#include <map>
#include <csignal>
#include "Config.hpp"
#include "Socket.hpp"
#include "Events.hpp"
#include "Connection.hpp"


class Daemon {
private:
    Config                            *config;
    std::map<int, IEventSubscriber *> subscriber;
//    std::map<int, Socket *>     sockets;
    std::map<int, Connection *>       connections;
    Events                            *events;

    static void registerSignal();
    void removeExpiredConnections();
    void unsubscribeConnection(Connection *connection);
public:
    enum Status {
        UNUSED         = 0,
        AWAIT_NEW_REQ  = 1,
        CGI_PROCESSING = 2,
        SENDING        = 3,
        TIMEOUT        = 4,
        CLOSING        = 5
    };

    Daemon(Config *config, const std::map<int, Socket *> &sockets, Events *events);
    void run();
    void processEvent(Connection *connection, int fd, size_t bytes_available, int16_t filter, bool eof);
};

#endif //WEBSERV_DAEMON_HPP
