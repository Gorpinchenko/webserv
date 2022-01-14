#ifndef WEBSERV_CONFIG_HPP
#define WEBSERV_CONFIG_HPP

#include <iostream>
#include <vector>
#include "Server.hpp"

class Config {
private:
    std::vector<Server *> servers;
    void parse(const std::string &path_to_file);

//    Config &operator=(const Config &other);
//    Config(const Config &other);
public:
    Config(const std::string &path_to_file);
    std::vector<Server *> getServers() const;

    class ConfigException : public std::exception {
        const std::string msg;
    public:
        ConfigException(const std::string &msg);
        ~ConfigException() throw();
        const char *what() const throw();
    };
};


#endif //WEBSERV_CONFIG_HPP
