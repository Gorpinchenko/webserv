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
#include "utils/Print.hpp"
#include "Socket.hpp"

class Connection : public IEventSubscriber {
private:
    int             connection_fd;
    std::string     buffer;
    bool            keep_alive;
    short           status;
    time_t          connection_timeout;
    struct sockaddr s_addr;
//    unsigned long                      _max_body_size;
    std::string     chunk_length;
    unsigned long   c_bytes_left;
    short           skip_n;

    HttpRequest  *request;
    HttpResponse *response;
    std::vector<Server *>servers;

    static const int DEFAULT_TIMEOUT = 2;

    void parseRequestMessage(size_t &pos);
    void appendBody(size_t &pos);
    bool parseChunked(unsigned long &pos, unsigned long bytes);
    Server *getServer();
    std::string getIp();
public:
    enum Status {
        UNUSED         = 0,
        AWAIT_NEW_REQ  = 1, // if keep alive true close
        CGI_PROCESSING = 2,
        SENDING        = 3,// response ready -> send
        TIMEOUT        = 4,//should be closed with timeout;
        CLOSING        = 5
    };

    Connection(Socket *socket);
    ~Connection();

    int getConnectionFd() const;
    bool getKeepAlive() const;
    short getStatus() const;
    HttpRequest *getRequest() const;
    std::string &getBuffer();
    void clearBuffer();

    void parseRequest(size_t bytes);
    bool isShouldClose();
    void prepareResponse();
    void prepareResponseMessage();
    void processResponse(size_t bytes, bool eof);
    void processCgi(int fd, size_t bytes_available, int16_t filter, bool eof);

    class ConnectionException : public std::exception {
        const char *m_msg;
    public:
        explicit ConnectionException(const char *msg);

        ~ConnectionException() throw();

        const char *what() const throw();
    };

    bool writeCgi(size_t bytes, bool eof);

    bool readCgi(size_t bytes, bool eof);

    HttpResponse *getResponse() const;

    void setResponse(HttpResponse *_response);
};

#endif //WEBSERV_CONNECTION_HPP
