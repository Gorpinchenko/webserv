//#include "WebServ.hpp"
#include "Config.hpp"
#include "Socket.hpp"
#include "Events.hpp"
#include "Daemon.hpp"

void run(const std::string &path_to_file) {
    Config                  *config;
    Events                  *events;
    std::map<int, Socket *> sockets;

    config = new Config(path_to_file);
    events = new Events(1000);

    std::vector<Server *> servers = config->getServers();

    if (!servers.empty()) {
        std::vector<Server *>::iterator begin;
        std::vector<Server *>::iterator end;
        begin = servers.begin();
        end   = servers.end();

        while (begin != end) {
            Socket *socket;
            int    socket_fd;

//            char      str[INET_ADDRSTRLEN];//todo delete
//            in_addr_t host = (*begin)->getHost();//todo delete
//            std::cout << "host: " << inet_ntop(AF_INET, &host, str, INET_ADDRSTRLEN) << " port: " << (*begin)->getPort() << std::endl;//todo delete
            socket    = new Socket((*begin));
            socket_fd = socket->getFd();

            sockets.insert(std::map<int, Socket *>::value_type(socket_fd, socket));
            events->subscribe(socket_fd, EVFILT_READ);
            begin++;
        }

        Daemon daemon(config, sockets, events);
        daemon.run();
    }
}

int main(int argc, char **argv) {
    std::string path_to_file = "./config/default.conf";

    if (argc > 1) {
        path_to_file = argv[1];
    }

    try {
        run(path_to_file);
    } catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
}
