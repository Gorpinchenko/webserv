#ifndef WEBSERV_EVENTS_HPP
#define WEBSERV_EVENTS_HPP

#include <iostream>
#include <set>
#include <sys/event.h>
#include <cerrno>

class Events {
private:
    const int     max_size;
    int           queue_fd;
    std::set<int> fds;
    struct kevent *w_event;
    struct kevent *res_event;
public:
    Events(int max_size);
    void subscribe(int fd, short type);
    std::pair<int, struct kevent *> getUpdates(int = 5);
};


#endif //WEBSERV_EVENTS_HPP
