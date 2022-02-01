#ifndef WEBSERV_CGI_HPP
#define WEBSERV_CGI_HPP

# include <string>
# include <map>
# include "HttpRequest.hpp"
# include "Location.hpp"

class HttpRequest;

class Cgi {
    std::string                         _path;
    std::map<std::string, std::string>	_env;
    char**                              _arr;

    int                                 _reqFd;
    int                                 _resFd;
    pid_t                               _pid;
    size_t                              _pos;
    bool                                _headers;

public:
    explicit Cgi(const std::string& path);
    ~Cgi();

    const std::string &getPath() const;

    const std::map<std::string, std::string> &getEnv() const;

    char **getEnvAsArray(HttpRequest *request, std::string ip, std::string path, uint16_t port);

    pid_t getPid() const;

    void setPid(pid_t pid);

    int getReqFd() const;

    void setReqFd(int reqFd);

    int getResFd() const;

    void setResFd(int resFd);

    size_t getPos() const;

    void setPos(size_t pos);

    bool isHeaders() const;

    void setHeaders(bool headers);

};


#endif //WEBSERV_CGI_HPP
