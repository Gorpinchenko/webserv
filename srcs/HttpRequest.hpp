#ifndef WEBSERV_HTTPREQUEST_HPP
#define WEBSERV_HTTPREQUEST_HPP

#include <map>
#include "AHttpMessage.hpp"
#include "HttpResponse.hpp"

class HttpRequest : public AHttpMessage {
private:
    std::string                        method;
    std::string                        request_uri;
    std::string                        query_string;
    std::string                        uri_no_query;
//    std::string                        normalized_path;
    std::string                        http_v;
    bool                               chunked;
    std::map<std::string, std::string> header_fields;
    std::string                        body;
    unsigned long                      content_length;
    unsigned long                      max_body_size;
    bool                               ready;
    uint16_t                           parsing_error;

public:
    enum Limits {
        MAX_METHOD            = 8,
        MAX_URI               = 2000,
        MAX_V                 = 100,
        MAX_HEADERS           = 100,
        MAX_NAME              = 100,
        MAX_VALUE             = 1000,
        MAX_MESSAGE           = 10000,
        MAX_DEFAULT_BODY_SIZE = 1000000
    };

    HttpRequest();

    void setReady(bool ready);
    bool getReady() const;
    const std::string &getRequestUri() const;
    void setParsingError(uint16_t status);
    uint16_t getParsingError() const;
    unsigned long getContentLength() const;
    void setContentLength(unsigned long len);
    bool getChunked() const;
    std::map<std::string, std::string> &getHeaders();
    void setHeader(std::string key, std::string value);
    std::string &getBody();
    unsigned long getMaxBodySize() const;

    bool headersSent(const std::string &req);
    bool parseRequestLine(const std::string &req, size_t &pos);
    bool parseHeaders(const std::string &buffe, size_t &pos);
    bool processUri();
    bool processHeaders();
    bool checkContentLength();
};

#endif //WEBSERV_HTTPREQUEST_HPP
