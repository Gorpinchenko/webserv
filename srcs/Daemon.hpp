#ifndef WEBSERV_DAEMON_HPP
#define WEBSERV_DAEMON_HPP

#include <iostream>
#include <map>
#include <csignal>
#include "Config.hpp"
#include "Socket.hpp"
#include "Events.hpp"


class Daemon {
private:
    Config                  *config;
    std::map<int, Socket *> sockets;
    Events                  *events;

    static void registerSignal();
public:
    Daemon(Config *config, const std::map<int, Socket *> &sockets, Events *events);
    void run();
};


#endif //WEBSERV_DAEMON_HPP
