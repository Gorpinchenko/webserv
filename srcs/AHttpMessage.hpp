#ifndef WEBSERV_AHTTPMESSAGE_HPP
#define WEBSERV_AHTTPMESSAGE_HPP

#include <iostream>

class AHttpMessage {
protected:
    bool parse(const std::string &src, std::size_t &token_start, const std::string &token_delim, bool delim_exact,
               std::size_t limit, std::string &buff);//todo преписать
};


#endif //WEBSERV_AHTTPMESSAGE_HPP
