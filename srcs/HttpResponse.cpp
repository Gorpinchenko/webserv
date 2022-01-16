#include "HttpResponse.hpp"

HttpResponse::HttpResponse(HttpResponse::HTTPStatus status) {
    (void)status;
    //    this->config = nullptr;
    //    this->cgi = nullptr;
    this->pos = 0;
//    setError(status, nullptr);
}
