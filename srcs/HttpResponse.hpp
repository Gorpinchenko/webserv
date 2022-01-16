#ifndef WEBSERV_HTTPRESPONSE_HPP
#define WEBSERV_HTTPRESPONSE_HPP


#include "AHttpMessage.hpp"

class HttpResponse: public AHttpMessage {
private:
//    std::string                        _proto;
//    u_int16_t                          _status_code;
//    std::string                        _status_reason;
//    std::string                        _response_string;
//    std::string                        _absolute_path;
//    //	const VirtualServer *_serv;
//    std::map<std::string, std::string> _response_headers;
//    std::string                        _body;
//    std::size_t                        _body_size;
//    CgiHandler                         *_cgi;
//    std::vector<char>                  _headers_vec;
    size_t                             pos;
//    const VirtualServer                *_config;
//    const Location                           *_loc;
public:
    enum HTTPStatus {
        HTTP_CONTINUE                        = 100,
        HTTP_SWITCHING_PROTOCOLS             = 101,
        HTTP_PROCESSING                      = 102,
        HTTP_OK                              = 200,
        HTTP_CREATED                         = 201,
        HTTP_ACCEPTED                        = 202,
        HTTP_NONAUTHORITATIVE                = 203,
        HTTP_NO_CONTENT                      = 204,
        HTTP_RESET_CONTENT                   = 205,
        HTTP_PARTIAL_CONTENT                 = 206,
        HTTP_MULTI_STATUS                    = 207,
        HTTP_ALREADY_REPORTED                = 208,
        HTTP_IM_USED                         = 226,
        HTTP_MULTIPLE_CHOICES                = 300,
        HTTP_MOVED_PERMANENTLY               = 301,
        HTTP_FOUND                           = 302,
        HTTP_SEE_OTHER                       = 303,
        HTTP_NOT_MODIFIED                    = 304,
        HTTP_USE_PROXY                       = 305,
        HTTP_USEPROXY                        = 305, /// @deprecated
        // UNUSED: 306
        HTTP_TEMPORARY_REDIRECT              = 307,
        HTTP_PERMANENT_REDIRECT              = 308,
        HTTP_BAD_REQUEST                     = 400,
        HTTP_UNAUTHORIZED                    = 401,
        HTTP_PAYMENT_REQUIRED                = 402,
        HTTP_FORBIDDEN                       = 403,
        HTTP_NOT_FOUND                       = 404,
        HTTP_METHOD_NOT_ALLOWED              = 405,
        HTTP_NOT_ACCEPTABLE                  = 406,
        HTTP_PROXY_AUTHENTICATION_REQUIRED   = 407,
        HTTP_REQUEST_TIMEOUT                 = 408,
        HTTP_CONFLICT                        = 409,
        HTTP_GONE                            = 410,
        HTTP_LENGTH_REQUIRED                 = 411,
        HTTP_PRECONDITION_FAILED             = 412,
        HTTP_REQUEST_ENTITY_TOO_LARGE        = 413,
        HTTP_REQUESTENTITYTOOLARGE           = 413, /// @deprecated
        HTTP_REQUEST_URI_TOO_LONG            = 414,
        HTTP_REQUESTURITOOLONG               = 414, /// @deprecated
        HTTP_UNSUPPORTED_MEDIA_TYPE          = 415,
        HTTP_UNSUPPORTEDMEDIATYPE            = 415, /// @deprecated
        HTTP_REQUESTED_RANGE_NOT_SATISFIABLE = 416,
        HTTP_EXPECTATION_FAILED              = 417,
        HTTP_IM_A_TEAPOT                     = 418,
        HTTP_ENCHANCE_YOUR_CALM              = 420,
        HTTP_MISDIRECTED_REQUEST             = 421,
        HTTP_UNPROCESSABLE_ENTITY            = 422,
        HTTP_LOCKED                          = 423,
        HTTP_FAILED_DEPENDENCY               = 424,
        HTTP_UPGRADE_REQUIRED                = 426,
        HTTP_PRECONDITION_REQUIRED           = 428,
        HTTP_TOO_MANY_REQUESTS               = 429,
        HTTP_REQUEST_HEADER_FIELDS_TOO_LARGE = 431,
        HTTP_UNAVAILABLE_FOR_LEGAL_REASONS   = 451,
        HTTP_INTERNAL_SERVER_ERROR           = 500,
        HTTP_NOT_IMPLEMENTED                 = 501,
        HTTP_BAD_GATEWAY                     = 502,
        HTTP_SERVICE_UNAVAILABLE             = 503,
        HTTP_GATEWAY_TIMEOUT                 = 504,
        HTTP_VERSION_NOT_SUPPORTED           = 505,
        HTTP_VARIANT_ALSO_NEGOTIATES         = 506,
        HTTP_INSUFFICIENT_STORAGE            = 507,
        HTTP_LOOP_DETECTED                   = 508,
        HTTP_NOT_EXTENDED                    = 510,
        HTTP_NETWORK_AUTHENTICATION_REQUIRED = 511
    };

    HttpResponse(HttpResponse::HTTPStatus status);
};


#endif //WEBSERV_HTTPRESPONSE_HPP
