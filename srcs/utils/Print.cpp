#include "Print.hpp"

//std::ostream &operator<<(std::ostream &os, Code cod) {
//    os << "\033[" << (int) cod << "m";
//    return os;
//
//}

std::ostream &operator<<(std::ostream &out, HttpRequest &c) {
//    if (LOG_LEVEL > 0) {
    out << "\033[0;31m" << ">>>> [REQUEST] <<<<" << std::endl;
    out << c.getRequestUri() << std::endl;
//    }
//    if (LOG_LEVEL > 1) {
    out << "parsing error " << c.getParsingError() << std::endl;
    std::map<std::string, std::string>                      res = c.getHeaders();
    for (std::map<std::string, std::string>::const_iterator it  = res.begin(); it != res.end(); ++it) {
        out << it->first << ": " << it->second << std::endl;
    }
    out << std::endl;
    out << "Content-Length is set to " << c.getContentLength() << std::endl;
//    }
//    if (LOG_LEVEL > 2)
    out << c.getBody().data() << std::endl;

//    if (LOG_LEVEL > 0)
    out << "\033[0m" << std::endl;
    return out;
}


//std::ostream &operator<<(std::ostream &out, const HttpResponse &c) {
//    if (LOG_LEVEL > 0) {
//        out << FG_LIGHT_GREEN << ">>>> [RESPONSE] <<<<" << std::endl;
//        out << c.getResponseString() << std::endl;
//    }
//    if (LOG_LEVEL > 1) {
//        out << "status code: [" << c.getStatusCode() << "] " <<
//        HttpResponse::getReasonForStatus(static_cast<HttpResponse::HTTPStatus>(c.getStatusCode())) << std::endl;
//        const std::map<std::string, std::string>                &res = c.getHeader();
//        for (std::map<std::string, std::string>::const_iterator it   = res.begin(); it != res.end(); ++it) {
//            out << it->first << ": " << it->second << std::endl;
//        }
//        out << std::endl;
//        out << "Body size is: " << c.getBody().size() << std::endl;
//    }
//    if (LOG_LEVEL > 2)
//        out << c.getBody() << std::endl;
//
//    if (LOG_LEVEL > 0)
//        out << FG_DEFAULT << std::endl;
//    return out;
//}

//std::ostream &operator<<(std::ostream &out, const Session &c) {
//    if (LOG_LEVEL > 0) {
//        out << FG_LIGHT_CYAN <<  ">>>> [Session] <<<<" << std::endl;
//        out << "Ip: " << c.getClientIp() << " port: " << c.getClientPort() << std::endl;
//        //        out << "fd " << c.getFd() << std::endl;
//    }
//    if (LOG_LEVEL > 1) {
//        time_t t = c.getConnectionTimeout();
//        char   buf[100];
//        std::strftime(buf, 100, "%a, %d %b %Y %H:%M:%S %Z", gmtime(&t));
//        out << "Last updated: " << buf << std::endl;
//    }
//    if (LOG_LEVEL > 0)
//        out << FG_DEFAULT << std::endl;
//    return out;
//}
