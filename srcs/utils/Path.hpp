#ifndef WEBSERV_PATH_HPP
#define WEBSERV_PATH_HPP

#include <string>
#include <sys/stat.h>
#include <dirent.h>
#include "FileStats.hpp"

class Path {
public:
    static const std::string AUTOINDEX_HTML;
    static bool isDirectory(const std::string &path);
    static std::string getAutoIndexHtml(const std::string &path, const std::string &uri_path);
};

#endif //WEBSERV_PATH_HPP
