#include "Cgi.hpp"

Cgi::Cgi(const std::string& path)
: _path(path), _env(), _headers_parsed(false) {
    _pid = -1;
    _reqFd = -1;
    _resFd = -1;
    _pos = 0;
}

Cgi::~Cgi() {}

const std::string &Cgi::getPath() const {
    return _path;
}

const std::map<std::string, std::string> &Cgi::getEnv() const {
    return _env;
}

bool Cgi::isHeadersParsed() const {
    return _headers_parsed;
}

void Cgi::setHeadersParsed(bool status) {
    _headers_parsed = status;
}

char **Cgi::getEnvAsArray(HttpRequest *request, std::string ip, std::string path, uint16_t port) {
    std::map<std::string, std::string> headers = request->getHeaders();

    if (!headers.count("Auth-Scheme") && !headers["Auth-Scheme"].empty()) {
        _env["AUTH_TYPE"] = headers["Authorization"];
    }
    _env["REDIRECT_STATUS"] = "CGI";
    _env["GATEWAY_INTERFACE"] = "CGI/1.1";

    _env["SCRIPT_NAME"] = path;
    _env["SCRIPT_FILENAME"] = request->getAbsolutPath();
    _env["REQUEST_METHOD"] = request->getMethod();
    _env["CONTENT_LENGTH"] = std::to_string(request->getContentLength());
    if (headers.count("Content-Type")) {
        _env["CONTENT_TYPE"] = headers["Content-Type"];
    }
    _env["PATH_INFO"] = request->getUriNoQuery();
    _env["PATH_TRANSLATED"] = request->getUriNoQuery();
    _env["QUERY_STRING"] = request->getQueryString();
    _env["REMOTE_ADDR"] = ip;
    _env["REMOTE_HOST"] = ip;
    if (headers.count("Authorization")) {
        _env["REMOTE_IDENT"] =  headers["Authorization"];
        _env["REMOTE_USER"] =  headers["Authorization"];
    }
    _env["REQUEST_URI"] = request->getRequestUri();
    if (headers.count("Host") && !headers["Host"].empty()) {
        _env["SERVER_NAME"] = headers["Host"];
    } else {
        _env["SERVER_NAME"] = _env["REMOTE_ADDR"];
    }
    _env["SERVER_PORT"] = std::to_string(port);
    _env["SERVER_PROTOCOL"] = "HTTP/1.1";
    _env["SERVER_SOFTWARE"] = "Weebserv/1.0";

    std::map<std::string, std::string>           tmp_map;
    std::map<std::string, std::string>::iterator tmp_map_iter;
    std::map<std::string, std::string>::iterator it;

    for (it = request->getHeaders().begin(); it != request->getHeaders().end();) {
        std::string tmp = it->first;
        std::replace(tmp.begin(), tmp.end(), '-', '_');
        std::transform(tmp.begin(), tmp.end(), tmp.begin(), ::toupper);
        tmp_map["HTTP_" + tmp] = it->second;
        tmp_map_iter = it;
        ++tmp_map_iter;
        request->getHeaders().erase(it);
        it = tmp_map_iter;
    }
    _env.insert(tmp_map.begin(), tmp_map.end());

    char	**env = new char*[this->_env.size() + 1];
    int	    j = 0;

    for (std::map<std::string, std::string>::const_iterator i = this->_env.begin(); i != this->_env.end(); i++) {
        std::string	element = i->first + "=" + i->second;
        env[j] = new char[element.size() + 1];
        env[j] = strcpy(env[j], (const char*)element.c_str());
        j++;
    }
    env[j] = NULL;
    return env;
}

int Cgi::getReqFd() const {
    return _reqFd;
}

void Cgi::setReqFd(int reqFd) {
    _reqFd = reqFd;
}

int Cgi::getResFd() const {
    return _resFd;
}

void Cgi::setResFd(int resFd) {
    _resFd = resFd;
}

pid_t Cgi::getPid() const {
    return _pid;
}

void Cgi::setPid(pid_t pid) {
    _pid = pid;
}

size_t Cgi::getPos() const {
    return _pos;
}

void Cgi::setPos(size_t pos) {
    _pos = pos;
}
