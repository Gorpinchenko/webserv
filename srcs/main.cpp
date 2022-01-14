//#include "WebServ.hpp"
#include "Config.hpp"
#include "Socket.hpp"
#include "Events.hpp"
#include "Daemon.hpp"

void run(std::string path_to_file) {
    Config *config = new Config(path_to_file);
    Events *events = new Events(1000);
    std::map < int, Socket * > sockets;

    std::cout << "run WebServ" << std::endl;
    std::vector < Server * > servers = config->getServers();

    if (servers.size() != 0) {
        std::vector<Server *>::iterator begin = servers.begin();
        std::vector<Server *>::iterator end   = servers.end();

        while (begin != end) {
            Socket *socket   = new Socket((*begin)->getHost(), (*begin)->getPort());
            int    socket_fd = socket->getSocketFd();

            sockets.insert(std::map<int, Socket *>::value_type(socket_fd, socket));
            events->subscribe(socket_fd, EVFILT_READ);
            //            subscribe(socket_fd, EVFILT_READ);
            begin++;
        }
        std::cout << "run WebServ" << std::endl;
    }

    Daemon daemon(config, sockets, events);
    daemon.run();
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
