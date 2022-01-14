#include "Daemon.hpp"

Daemon::Daemon(Config *config, std::map<int, Socket *> sockets, Events *events) :
        config(config),
        sockets(sockets),
        events(events) {
    (void)this->config;
    (void)this->sockets;
    (void)this->events;
}

void signal_handler(int signal) {
    std::cout << "stopping on signal " << signal << std::endl;
    exit(signal);
}

void Daemon::registerSignal() {
    signal(SIGPIPE, SIG_IGN);
    std::signal(SIGINT, signal_handler);
    std::signal(SIGQUIT, signal_handler);
}

void Daemon::run() {
    std::cout << "run daemon" << std::endl;
    std::pair<int, struct kevent *>        updates;
//    std::map<int, ISubscriber *>::iterator it;
    int                                    i;
    size_t                                 bytes_available;
    int                                    cur_fd;
    int16_t                                filter;
    uint16_t                               flags;
    uint32_t                               fflags;

    this->registerSignal();

    while (true) {
        updates = events->getUpdates();
        i       = 0;
        while (i < updates.first) {
//            it              = _subs.find(updates.second[i].ident);
            filter          = updates.second[i].filter;
            cur_fd          = updates.second[i].ident;
            flags           = updates.second[i].flags;
            fflags          = updates.second[i].fflags;
            bytes_available = updates.second[i].data;
            if (flags & EV_ERROR) {   /* report any error */
                fprintf(stderr, "EV_ERROR: %s\n", strerror(bytes_available));
            }
//            if (it != _subs.end()) {
//                it->second->processEvent(cur_fd, bytes_available, filter, fflags, (flags & EV_EOF), this);
//            }
            ++i;
        }
//        removeExpiredSessions();
    }

}
