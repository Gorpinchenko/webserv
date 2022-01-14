#ifndef WEBSERV_SERVER_HPP
#define WEBSERV_SERVER_HPP

#include <iostream>

class Server {
private:
    in_addr_t                        host;
    uint16_t                         port;
//    std::string                      server_name;
//    std::map<short, std::string>     error_pages;
//    std::map <std::string, Location> locations;
//    unsigned long                    body_size_limit;
public:
    in_addr_t getHost() const;
    uint16_t getPort() const;


};


#endif //WEBSERV_SERVER_HPP
