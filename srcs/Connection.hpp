#ifndef WEBSERV_CONNECTION_HPP
#define WEBSERV_CONNECTION_HPP

#include <iostream>
#include <map>
#include <unistd.h>
#include <fcntl.h>
#include <arpa/inet.h>
//#include <sys/socket.h>
#include <sys/event.h>
#include "IEventSubscriber.hpp"
#include "HttpRequest.hpp"
#include "HttpResponse.hpp"
#include "Print.hpp"

class Connection : public IEventSubscriber {
private:
    int          connection_fd;
    std::string  buffer;
    HttpRequest  *request;
    HttpResponse *response;

    bool          keep_alive;
    short         status;
    time_t        connection_timeout;
//    unsigned long                      _max_body_size;
    std::string   chunk_length;
    unsigned long c_bytes_left;
    short         skip_n;

    static const int DEFAULT_TIMEOUT = 2;
public:
    enum Status {
        UNUSED         = 0,
        AWAIT_NEW_REQ  = 1, // if keep alive true close
        CGI_PROCESSING = 2,
        SENDING        = 3,// response ready -> send
        TIMEOUT        = 4,//should be closed with timeout;
        CLOSING        = 5
    };

    Connection(int socket_fd);

    int getConnectionFd() const;
    bool getKeepAlive() const;
    short getStatus() const;
    HttpRequest *getRequest() const;
    std::string &getBuffer();
    void clearBuffer();

    void parseRequest(size_t bytes);
    bool isShouldClose();
    void parseRequestMessage(size_t &pos);
    void appendBody(size_t &pos);
    bool parseChunked(unsigned long &pos, unsigned long bytes);
    bool findRouteSetResponse();

    class ConnectionException : public std::exception {
        const char *m_msg;
    public:
        explicit ConnectionException(const char *msg);

        ~ConnectionException() throw();

        const char *what() const throw();
    };
};

#endif //WEBSERV_CONNECTION_HPP
