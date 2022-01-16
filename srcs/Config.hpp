#ifndef WEBSERV_CONFIG_HPP
# define WEBSERV_CONFIG_HPP

# include <vector>
# include <stdint.h>
# include <netinet/in.h>
# include <unistd.h>
# include <fstream>
# include <iterator>
# include <sstream>
# include <algorithm>
# include <arpa/inet.h>
# include "Server.hpp"
# include "IDirective.hpp"

class Config
{
private:
    std::string _path_to_file;
    std::vector<Server *> _servers;
    void parse(const std::string &path_to_file);

    Config &operator=(const Config &other);
    Config(const Config &other);

public:
    Config(const std::string &path_to_file);
    ~Config();

    const std::vector<Server *> &getServers() const;

    class ConfigException : public std::exception
    {
    private:
        std::string msg;
    public:
        ConfigException(const std::string &msg);
        ~ConfigException() throw();
        const char *what() const throw();
    };

    bool isComment(std::string const &str) const;

    bool isDirectiveLine(std::string const &str) const;

    void parseServerData(
        std::vector<std::string>::iterator begin,
        std::vector<std::string>::iterator end);

    void checkTrashDirective(std::string const &name) const;

    void parseListen(
        IDirective *server,
        std::vector<std::string>::iterator &value,
        std::vector<std::string>::iterator end);

    void parsePort(
        IDirective *server,
        std::vector<std::string>::iterator &value,
        std::vector<std::string>::iterator end);

    void parseServerName(
        IDirective *server,
        std::vector<std::string>::iterator &value,
        std::vector<std::string>::iterator end);

    void parseClientMaxBodySize(
        IDirective *server,
        std::vector<std::string>::iterator &value,
        std::vector<std::string>::iterator end);

    void parseMimeConfPath(
        IDirective *server,
        std::vector<std::string>::iterator &value,
        std::vector<std::string>::iterator end);

    void parseErrorPages(
        IDirective *server,
        std::vector<std::string>::iterator &value,
        std::vector<std::string>::iterator end);

    void parseLocation(
        IDirective *server,
        std::vector<std::string>::iterator &value,
        std::vector<std::string>::iterator end);

    void parseRedirection(
        IDirective *server,
        std::vector<std::string>::iterator &value,
        std::vector<std::string>::iterator end);

    void parseRoot(
        IDirective *server,
        std::vector<std::string>::iterator &value,
        std::vector<std::string>::iterator end);

    void parseMethods(
        IDirective *server,
        std::vector<std::string>::iterator &value,
        std::vector<std::string>::iterator end);
};


#endif //WEBSERV_CONFIG_HPP
