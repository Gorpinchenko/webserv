#include "Server.hpp"


in_addr_t Server::getHost() const
{
    return host;
}

uint16_t Server::getPort() const
{
    return port;
}
