#include "Connection.hpp"


Connection::Connection(Socket *socket)
        : keep_alive(false),
          status(UNUSED),
          connection_timeout(),
          chunk_length(), //todo поменять название или вынести отсюда
          c_bytes_left(0), //todo поменять название или вынести отсюда
          skip_n(0), //todo поменять название или вынести отсюда
          request(nullptr),
          response(nullptr),
          servers(socket->getServers()) {
    std::time(&this->connection_timeout);
    int       new_fd;
    socklen_t s_len;

    new_fd = accept(socket->getFd(), &s_addr, &s_len);
    if (new_fd < 0) {
        throw ConnectionException(strerror(errno));
    }
    this->connection_fd = new_fd;
}

Connection::~Connection() {
    if (this->response != nullptr) {
        delete this->response;
        this->response = nullptr;
    }
    if (this->request != nullptr) {
        delete this->request;
        this->request = nullptr;
    }
}

int Connection::getConnectionFd() const {
    return this->connection_fd;
}

bool Connection::getKeepAlive() const {
    return this->keep_alive;
}

short Connection::getStatus() const {
    return this->status;
}

void Connection::parseRequest(size_t bytes_available) {
    std::string res(bytes_available, 0);

    if (read(this->connection_fd, &res[0], bytes_available) < 0) {
//        end();
    }
    this->buffer.append(res);
//    std::cout << this->buffer.data() << std::endl;

    if (this->request == nullptr) {
        this->request = new HttpRequest();
    }
    size_t pos = 0;
    if (this->request->getRequestUri().empty()) {
        this->parseRequestMessage(pos);
    } else {
        this->appendBody(pos);
    }
    if (!this->request->getRequestUri().empty()) {
        std::map<std::string, std::string>::const_iterator it;
        it = this->request->getHeaders().find("Connection");
        if ((it != this->request->getHeaders().end() && it->second == "close") ||
            this->request->getParsingError() == HttpResponse::HTTP_BAD_REQUEST) {
            this->keep_alive = false;
        } else {
            this->keep_alive = true;
        }
    }
    std::time(&this->connection_timeout);
}

void Connection::prepareResponse() {
    if (!this->request->getReady()) {
        return;
    }
    this->buffer.clear();

//        std::cout << *this->request << std::endl;//todo мб сделать запись в консоль
    if (this->response == nullptr) {//todo потом удалить наверно
//        this->response = new HttpResponse(this->server, static_cast<HttpResponse::HTTPStatus>(this->request->getParsingError()));
        //TODO мб тут проверять, что нет правил для такого запроса
        this->response = new HttpResponse(this->getServer(), this->request);
    }

    this->prepareResponseMessage();

    if (this->keep_alive) {
        this->response->setHeader("Connection", "Keep-Alive");
        this->response->setHeader("Keep-Alive", "timeout=" + std::to_string(DEFAULT_TIMEOUT));
    }

    if (this->response->isCgi() && this->response->getStatusCode() == HttpResponse::HTTP_OK) {
        this->status = CGI_PROCESSING;
    } else {
        this->status = SENDING;
    }
}

std::string Connection::getIp() {
    char ip[INET6_ADDRSTRLEN];
    inet_ntop(AF_INET, &this->s_addr, ip, sizeof(ip));
    return std::string(ip);
}

void Connection::prepareResponseMessage() {
    if (this->response->getStatusCode() != HttpRequest::HTTP_OK && this->response->getStatusCode() != 0) {
        return;
    }
    /**
     * CGI process
     */
    if (this->response->isCgi()) {
        this->response->processCgiRequest(this->getIp());
    } else if (this->request->getMethod() == "GET") {
        this->response->processGetRequest();
    } else if (this->request->getMethod() == "POST") {
        this->response->processPostRequest();
    } else if (this->request->getMethod() == "DELETE") {
        this->response->processDeleteRequest();
    } else if (this->request->getMethod() == "PUT") {
        this->response->processPutRequest();
    }
}

void Connection::processResponse(size_t bytes, bool eof) {
    int res = 0;
    if (eof || (res = this->response->send(this->connection_fd, bytes)) == 1) {
//        std::cout << *this->response;
        if (!this->keep_alive) {
            this->status = CLOSING;
        } else {
            this->status = AWAIT_NEW_REQ;
        }
        if (this->request != nullptr) {
            delete this->request;
            this->request = nullptr;
        }
        if (this->response != nullptr) {
            delete this->response;
            this->response = nullptr;
        }
    } else if (res == -1) {
//        end();
    }
    std::time(&this->connection_timeout);
}

void Connection::processCgi(int fd, size_t bytes_available, int16_t filter, bool eof) {
    /**
     * Здесь что-то
     */
    if (filter == EVFILT_WRITE && fd == this->response->getCgi()->getReqFd()) {
        std::cout << "write" << std::endl;
        writeCgi(bytes_available, eof);
    } else if (filter == EVFILT_READ && fd == this->response->getCgi()->getResFd()) {
        std::cout << "read" << std::endl;
        readCgi(bytes_available, eof);
    }
}

bool Connection::writeCgi(size_t bytes, bool eof) {
    if (eof && this->response->getCgi()->getPos() < this->request->getBody().size()) {
        return (true);
    } else if (this->response->writeToCgi(this->request, bytes))
        return (true);
    return (false);
}

bool Connection::readCgi(size_t bytes, bool eof) {
    if (this->response->readCgi(bytes, eof)) {
        return (true);
    } else
        return (false);

}

void Connection::parseRequestMessage(size_t &pos) {
    const std::string &buff = this->getBuffer();
//    std::cout << "Connection::parseRequestMessage " << std::endl;
    (this->request->headersSent(buff)
     && this->request->parseRequestLine(buff, pos)
     && this->request->parseHeaders(buff, pos)
     && this->request->processUri()
     && this->request->processHeaders()
     && this->request->checkContentLength());

    if (this->request->getParsingError() != HttpResponse::HTTP_OK
        && this->request->getParsingError() != HttpResponse::HTTP_REQUEST_ENTITY_TOO_LARGE) {
        return;
    }
    if (this->request->getRequestUri().empty()) {
        return;
    }
    if (this->request->getContentLength() != 0 || this->request->getChunked()) {
        this->appendBody(pos);
        return;
    }
    this->request->setReady(true);
}

void Connection::appendBody(size_t &pos) {
    const std::string &buff = this->getBuffer();
    if (pos >= buff.size()) {
        this->clearBuffer();
        return;
    }
    if (this->request->getChunked()) {
        if (this->parseChunked(pos, buff.size())) {
            std::map<std::string, std::string>::iterator m_it;
            m_it = this->request->getHeaders().find("Transfer-Encoding");
            if (m_it != this->request->getHeaders().end() && m_it->second.find("chunked") != std::string::npos) {
                this->request->setHeader("Content-Length", std::to_string(this->request->getBody().size()));
                this->request->getHeaders().erase(m_it);
            }
            this->request->setReady(true);
            this->request->setContentLength(this->request->getBody().size());
        }
    } else {
        size_t len;
        if (this->request->getParsingError() == HttpResponse::HTTP_REQUEST_ENTITY_TOO_LARGE &&
            this->request->getContentLength() > 0) {
            if (buff.size() - pos > this->request->getContentLength()) {
                this->request->setContentLength(0);
            } else {
                len = this->request->getContentLength() - buff.size() - pos;
                this->request->setContentLength(len);
            }
        } else if (buff.size() + this->request->getBody().size() - pos > this->request->getContentLength()) {
            len               = this->request->getContentLength() - this->request->getBody().size();
            std::string &body = this->request->getBody();//todo возможно это не будет работать как надо
            body.insert(this->request->getBody().end(), buff.begin() + pos, buff.begin() + pos + len);
        } else {
            std::string &body = this->request->getBody();//todo возможно это не будет работать как надо
            body.insert(this->request->getBody().end(), buff.begin() + pos, buff.end());
        }
        if (this->request->getBody().size() == this->request->getContentLength()) {
            this->request->setReady(true);
            return;
        }
        this->clearBuffer();
    }
}

bool Connection::parseChunked(unsigned long &pos, unsigned long bytes) {
    const std::string &request_buff = this->getBuffer();
    while (pos < bytes) {
        while (this->skip_n > 0 && pos < bytes) {
            ++pos;
            --this->skip_n;
        }
        if (this->c_bytes_left == 0) {
            if (request_buff.find("0\r\n\r\n", pos) == pos) {
                this->request->setReady(true);
                return true;
            }
            if (request_buff.find("\r\n", pos) == std::string::npos) {
                std::string &t = this->getBuffer();
                t.erase(t.begin(), t.begin() + pos);
                return false;
            }
            while (pos < bytes && isxdigit(request_buff[pos]) && (this->chunk_length.size() < 8)) {
                this->chunk_length += request_buff[pos];
                ++pos;
            }
            if (pos < bytes && request_buff[pos] != ';' && request_buff[pos] != '\r') {
                this->request->setParsingError(HttpResponse::HTTP_BAD_REQUEST);
                return true;
            }
            this->c_bytes_left = strtoul(this->chunk_length.data(), nullptr, 16);
            this->chunk_length.clear();
            if (this->c_bytes_left == 0) {
                if (errno == ERANGE) {
                    this->request->setParsingError(HttpResponse::HTTP_REQUEST_ENTITY_TOO_LARGE);
//                    _parsing_error = HttpResponse::HTTP_REQUEST_ENTITY_TOO_LARGE;
                    return (true);
                } else {
                    std::string &tmp = this->getBuffer();
                    tmp.erase(tmp.begin(), tmp.begin() + --pos);
                    return (false);
                }
//            } else if (this->c_bytes_left + this->getRequest()->getBody().size() > _max_body_size) {
            } else if (this->c_bytes_left + this->getRequest()->getBody().size() >
                       this->getRequest()->getMaxBodySize()) {
                this->request->setParsingError(HttpResponse::HTTP_REQUEST_ENTITY_TOO_LARGE);
//                _parsing_error = HttpResponse::HTTP_REQUEST_ENTITY_TOO_LARGE;
            }
            this->skip_n += 2;
        }
        while (this->skip_n > 0 && pos < bytes) {
            ++pos;
            --this->skip_n;
        }
        if (pos < bytes) {
            unsigned long end;
            if (this->c_bytes_left >= bytes - pos) {
                end = bytes - pos;
            } else {
                end = this->c_bytes_left;
            }
            if (this->request->getParsingError() == HttpResponse::HTTP_OK) {
                this->getRequest()->getBody().append(request_buff.data() + pos, end);
//                _body.append(request_buff.data() + pos, end);
            }
            this->c_bytes_left -= end;
            pos += end;
        }
        if (this->c_bytes_left == 0) {
            this->skip_n += 2;
        }
    }
    if (pos == bytes) {
        this->clearBuffer();
        this->chunk_length.empty();
        return (false);
    }
    return (false);
}

bool Connection::isShouldClose() {
    if ((!this->keep_alive && this->status == AWAIT_NEW_REQ) || this->status == CLOSING) {
        return true;
    }

    time_t cur_time;
    std::time(&cur_time);

    if (std::difftime(cur_time, this->connection_timeout) > DEFAULT_TIMEOUT) {
        this->status = TIMEOUT;
        return true;
    }

    return false;
}

HttpRequest *Connection::getRequest() const {
    return this->request;
}

std::string &Connection::getBuffer() {
    return this->buffer;
}

void Connection::clearBuffer() {
    this->buffer.clear();
}

Server *Connection::getServer() {
    std::map<std::string, std::string>                 &headers = this->request->getHeaders();
    std::map<std::string, std::string>::const_iterator it;
    std::vector<Server *>::const_iterator              server_it;
    std::string                                        host;

    it = headers.find("Host");

    if (it == headers.end()) {
        return nullptr;
    }

    host = it->second.substr(0, it->second.find(":"));

    if (!host.empty()) {
        server_it = this->servers.begin();
        while (server_it != this->servers.end()) {
            if ((*server_it)->getServerName() == host) {
                return *server_it;
            }
            server_it++;
        }
    }

    return this->servers[0];
}

HttpResponse *Connection::getResponse() const {
    return response;
}

void Connection::setResponse(HttpResponse *_response) {
    Connection::response = _response;
}

Connection::ConnectionException::ConnectionException(const char *msg) : m_msg(msg) {}

Connection::ConnectionException::~ConnectionException() throw() {}

const char *Connection::ConnectionException::what() const throw() {
//    end();
    std::cerr << "ConnectionError: ";
    return this->m_msg;
}
