#ifndef WEBSERV_HTTPRESPONSE_HPP
#define WEBSERV_HTTPRESPONSE_HPP

//#include <map>
//#include <iostream>
//#include <cstdio>
#include<fstream>
//#include <fstream>
//#include <vector>
//#include <string>
//#include <cstddef>
//#include <sys/types.h>
//#include <sys/wait.h>

#include "AHttpMessage.hpp"
#include "HttpRequest.hpp"
#include "Server.hpp"
#include "./utils/Path.hpp"

class HttpRequest;

class HttpResponse : public AHttpMessage {
private:
    std::string       protocol;
    u_int16_t         status_code;
    std::string       status_reason;
    std::string       response_string;
//    std::string                        _absolute_path;
//    //	const VirtualServer *_serv;
//    std::map<std::string, std::string> _response_headers;
    std::string       body;
    std::size_t       body_size;
//    CgiHandler                         *_cgi;
    std::vector<char> _headers_vec;//todo delete
    size_t            pos;
//    const VirtualServer                *_config;
    const Location    *location;
    Server            *server;
    HttpRequest       *request;

    HTTPStatus writeFileToBuffer(const std::string &file_path);
    void setError(HTTPStatus code);
    void prepareData();//todo delete
public:


    static const std::string HTTP_REASON_CONTINUE;
    static const std::string HTTP_REASON_SWITCHING_PROTOCOLS;
    static const std::string HTTP_REASON_PROCESSING;
    static const std::string HTTP_REASON_OK;
    static const std::string HTTP_REASON_CREATED;
    static const std::string HTTP_REASON_ACCEPTED;
    static const std::string HTTP_REASON_NONAUTHORITATIVE;
    static const std::string HTTP_REASON_NO_CONTENT;
    static const std::string HTTP_REASON_RESET_CONTENT;
    static const std::string HTTP_REASON_PARTIAL_CONTENT;
    static const std::string HTTP_REASON_MULTI_STATUS;
    static const std::string HTTP_REASON_ALREADY_REPORTED;
    static const std::string HTTP_REASON_IM_USED;
    static const std::string HTTP_REASON_MULTIPLE_CHOICES;
    static const std::string HTTP_REASON_MOVED_PERMANENTLY;
    static const std::string HTTP_REASON_FOUND;
    static const std::string HTTP_REASON_SEE_OTHER;
    static const std::string HTTP_REASON_NOT_MODIFIED;
    static const std::string HTTP_REASON_USE_PROXY;
    static const std::string HTTP_REASON_TEMPORARY_REDIRECT;
    static const std::string HTTP_REASON_PERMANENT_REDIRECT;
    static const std::string HTTP_REASON_BAD_REQUEST;
    static const std::string HTTP_REASON_UNAUTHORIZED;
    static const std::string HTTP_REASON_PAYMENT_REQUIRED;
    static const std::string HTTP_REASON_FORBIDDEN;
    static const std::string HTTP_REASON_NOT_FOUND;
    static const std::string HTTP_REASON_METHOD_NOT_ALLOWED;
    static const std::string HTTP_REASON_NOT_ACCEPTABLE;
    static const std::string HTTP_REASON_PROXY_AUTHENTICATION_REQUIRED;
    static const std::string HTTP_REASON_REQUEST_TIMEOUT;
    static const std::string HTTP_REASON_CONFLICT;
    static const std::string HTTP_REASON_GONE;
    static const std::string HTTP_REASON_LENGTH_REQUIRED;
    static const std::string HTTP_REASON_PRECONDITION_FAILED;
    static const std::string HTTP_REASON_REQUEST_ENTITY_TOO_LARGE;
    static const std::string HTTP_REASON_REQUEST_URI_TOO_LONG;
    static const std::string HTTP_REASON_UNSUPPORTED_MEDIA_TYPE;
    static const std::string HTTP_REASON_REQUESTED_RANGE_NOT_SATISFIABLE;
    static const std::string HTTP_REASON_EXPECTATION_FAILED;
    static const std::string HTTP_REASON_IM_A_TEAPOT;
    static const std::string HTTP_REASON_ENCHANCE_YOUR_CALM;
    static const std::string HTTP_REASON_MISDIRECTED_REQUEST;
    static const std::string HTTP_REASON_UNPROCESSABLE_ENTITY;
    static const std::string HTTP_REASON_LOCKED;
    static const std::string HTTP_REASON_FAILED_DEPENDENCY;
    static const std::string HTTP_REASON_UPGRADE_REQUIRED;
    static const std::string HTTP_REASON_PRECONDITION_REQUIRED;
    static const std::string HTTP_REASON_TOO_MANY_REQUESTS;
    static const std::string HTTP_REASON_REQUEST_HEADER_FIELDS_TOO_LARGE;
    static const std::string HTTP_REASON_UNAVAILABLE_FOR_LEGAL_REASONS;
    static const std::string HTTP_REASON_INTERNAL_SERVER_ERROR;
    static const std::string HTTP_REASON_NOT_IMPLEMENTED;
    static const std::string HTTP_REASON_BAD_GATEWAY;
    static const std::string HTTP_REASON_SERVICE_UNAVAILABLE;
    static const std::string HTTP_REASON_GATEWAY_TIMEOUT;
    static const std::string HTTP_REASON_VERSION_NOT_SUPPORTED;
    static const std::string HTTP_REASON_VARIANT_ALSO_NEGOTIATES;
    static const std::string HTTP_REASON_INSUFFICIENT_STORAGE;
    static const std::string HTTP_REASON_LOOP_DETECTED;
    static const std::string HTTP_REASON_NOT_EXTENDED;
    static const std::string HTTP_REASON_NETWORK_AUTHENTICATION_REQUIRED;
    static const std::string HTTP_REASON_UNKNOWN;
//    static const std::string DATE;
//    static const std::string SET_COOKIE;

//HttpResponse(Server *server, HttpResponse::HTTPStatus status);
    HttpResponse(Server *server, HttpRequest *request);
    uint16_t getStatusCode() const;
    static const std::string &getReasonForStatus(HTTPStatus status);
    void setResponseString(HTTPStatus status);

    void processGetRequest();
    void processPostRequest();
    void processDeleteRequest();
    void processPutRequest();

    int send(int fd, size_t bytes);

//    void processDeleteRequest(const VirtualServer *pServer, HttpRequest *req);
};


#endif //WEBSERV_HTTPRESPONSE_HPP
