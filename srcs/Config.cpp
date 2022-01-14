#include "Config.hpp"

Config::Config(const std::string &path_to_file) {
    this->parse(path_to_file);
}

void Config::parse(const std::string &path_to_file) {
    (void) path_to_file;//todo delete
    (void) this->servers.size();//todo delete
    std::cout << "Config::parse - " << path_to_file << std::endl;//todo delete
    //todo parse, validate and set 'servers' from file
//    this->servers =
//throw std::runtime_error("Error while parsing config file" + "fsfdgf");
//    throw Config::ConfigException("Error while parsing config file");
}

std::vector<Server *> Config::getServers() const {
    return this->servers;
}

Config::ConfigException::ConfigException(const std::string &msg) : msg(msg) {}

Config::ConfigException::~ConfigException() throw() {}

const char *Config::ConfigException::what() const throw() {
    std::cerr << "ConfigError: " << this->msg << std::endl;
    return "";
}
