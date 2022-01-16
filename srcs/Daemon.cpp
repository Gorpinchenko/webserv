#include "Daemon.hpp"

Daemon::Daemon(Config *config, const std::map<int, Socket *> &sockets, Events *events) :
        config(config),
        events(events) {
    this->subscriber.insert(sockets.begin(), sockets.end());
    (void) this->config;
}

void signal_handler(int signal) {
    std::cout << "stopping on signal" << std::endl;
    exit(signal);
}

void Daemon::registerSignal() {
    signal(SIGPIPE, SIG_IGN);
    std::signal(SIGINT, signal_handler);
    std::signal(SIGQUIT, signal_handler);
}

void Daemon::run() {
    std::cout << "run daemon" << std::endl;//todo delete
    std::pair<int, struct kevent *>             updates;
    struct kevent                               event = {};
    std::map<int, IEventSubscriber *>::iterator sub_it;
    int                                         sub_fd;
    int                                         i;

    this->registerSignal();

    while (true) {
        updates = this->events->getUpdates();
        i       = 0;
        while (i < updates.first) {
            event  = updates.second[i];
            sub_fd = static_cast<int> (event.ident);
            sub_it = this->subscriber.find(sub_fd);

            if (event.flags & EV_ERROR) {   /* report any error */
                fprintf(stderr, "EV_ERROR: %s\n", strerror(static_cast<int>(event.data)));//todo мб это можно удалить
            }

            if (sub_it != this->subscriber.end()) {
                if (dynamic_cast<Socket *>(sub_it->second)) {
                    std::cout << "Socket " << std::endl;
                    Connection *connection;
                    int        connection_fd;

                    connection    = new Connection(sub_fd);
                    connection_fd = connection->getConnectionFd();
                    this->subscriber.insert(std::make_pair(connection_fd, connection));
                    this->connections.insert(std::make_pair(connection_fd, connection));
                    this->events->subscribe(connection_fd, EVFILT_READ);
                } else if (dynamic_cast<Connection *>(sub_it->second)) {
//                    std::cout << "Connection " << std::endl;
                    Connection *connection = dynamic_cast<Connection *>(sub_it->second);
//                    connection->parseRequest(event.data);

                    this->processEvent(connection, sub_fd, event.data, event.filter, (event.flags & EV_EOF));
//                    std::string res = "HTTP/1.1 200 OK\nDate: Mon, 27 Jul 2009 12:28:53 GMT\nServer: Apache/2.2.14 (Win32)\nLast-Modified: Wed, 22 Jul 2009 19:15:56 GMT\nContent-Length: 11\nContent-Type: text/html\nConnection: Closed\n\nhello world";
//                    send(sub_fd, res.data(), res.size(), 0);
//                    close(sub_fd);
                }
            }
            ++i;
        }
//        std::cout << "Events count:  " << this->events->getFds().size() << std::endl;
        this->removeExpiredConnections();
    }
}

void Daemon::processEvent(Connection *connection, int fd, size_t bytes_available, int16_t filter, bool eof) {

    short       status        = connection->getStatus();
    int         connection_fd = connection->getConnectionFd();
    HttpRequest *request      = connection->getRequest();

    // close if eof on write socket or eof on read while request was not parsed completely;
    if (connection_fd == fd && eof &&
        (filter == EVFILT_WRITE ||
         (filter == EVFILT_READ && (request == nullptr || !request->getReady()))
        )) {
        this->unsubscribeConnection(connection);
        return;
//        return end();
    } else if ((status == AWAIT_NEW_REQ || status == UNUSED) && filter == EVFILT_READ &&
               fd == connection_fd && bytes_available > 0) {
        connection->parseRequest(bytes_available);
        if (connection->getRequest()->getReady()) {
            std::cout << "requestReady " << std::endl;
//            prepareResponse();
        }
    }
//        else if (status == CGI_PROCESSING && filter == EVFILT_WRITE &&
//               fd == this->response->getCgi()->getRequestPipe()) {
//        writeCgi(bytes_available, eof);
//    }
//        else if (this->status == CGI_PROCESSING && filter == EVFILT_READ &&
//               fd == this->response->getCgi()->getResponsePipe()) {
//        readCgi(bytes_available, eof);
//    }
    else if (status == SENDING && filter == EVFILT_WRITE && fd == connection_fd) {
//        //        processResponse(bytes_available, eof);
    }
//status   = connection->getStatus();
//    if (prev_status != this->status) {
//        //        processPreviousStatus(prev_status);
//        //        processCurrentStatus(this->status);
//    }
}

void Daemon::removeExpiredConnections() {
    if (this->connections.empty()) {
        return;
    }

    std::vector<Connection *> to_delete;

    std::map<int, Connection *>::iterator connection_it;

    for (connection_it = this->connections.begin(); connection_it != this->connections.end();) {
        if (connection_it->second->isShouldClose()) {
            std::cout << "close Connection " << connection_it->second->getConnectionFd() << std::endl;
            std::cout << "\033[0;31m" << connection_it->second->getBuffer().data() << "\033[0m" << std::endl;
            to_delete.push_back(connection_it->second);
        }
        ++connection_it;
    }

    std::vector<Connection *>::iterator it;
    for (it = to_delete.begin(); it != to_delete.end(); ++it) {
        this->unsubscribeConnection((*it));
    }
}

void Daemon::unsubscribeConnection(Connection *connection) {
    this->events->unsubscribe(connection->getConnectionFd());
    close(connection->getConnectionFd());
    //            processPreviousStatus(_status);
    //            if(_response != nullptr && _response->getCgi() != nullptr) {
    //                this->events->unsubscribe(_response->getCgi()->getRequestPipe(), EVFILT_WRITE);
    //                this->events->unsubscribe(_response->getCgi()->getResponsePipe(), EVFILT_READ);
    //            }
    //            connection_it->second->close();

    std::map<int, IEventSubscriber *>::iterator sub_it;
    sub_it = this->subscriber.find(connection->getConnectionFd());
    if (sub_it != this->subscriber.end()) {
        this->subscriber.erase(sub_it);
    }

    std::map<int, Connection *>::iterator connection_it;
    connection_it = this->connections.find(connection->getConnectionFd());
    if (connection_it != this->connections.end()) {
        this->connections.erase(connection_it);
    }
}
