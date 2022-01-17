#include "Config.hpp"

//  Проверка на существование
//  Парсинг в вектор
//  Парсинг отдельных элементов
//  

std::string directives[16] = {
    "listen",
    "port",
    "server_name",
    "client_max_body_size",
    "mime_conf_path",
    "error_page",
    "location",
    "return",
    "root",
    "methods",
    "file_upload",
    "upload_tmp_path",
    "index",
    "autoindex",
    "cgi_pass",
    "server", // пока хз, мб проще выпилить
};

std::string methods[4] = {
    "GET",
    "POST",
    "DELETE",
    "PUT"
};

void (Config::*parseDirective[15])(
    IDirective *server,
    std::vector<std::string>::iterator &value,
    std::vector<std::string>::iterator &end) = {
    &Config::parseListen,
    &Config::parsePort,
    &Config::parseServerName,
    &Config::parseClientMaxBodySize,
    &Config::parseMimeConfPath,
    &Config::parseErrorPages,
    &Config::parseLocation,
    &Config::parseRedirection,
    &Config::parseRoot,
    &Config::parseMethods,
    &Config::parseFileUpload,
    &Config::parseUploadTmpPath,
    &Config::parseIndex,
    &Config::parseAutoindex,
    &Config::parseCgiPass
};

Config::Config(const std::string &path_to_file)
: _path_to_file(path_to_file)
{
    if (access(path_to_file.c_str(), F_OK) == -1)
        throw Config::ConfigException("config file " + path_to_file + " not found");
    this->parse(path_to_file);
}

Config::~Config()
{
    std::vector<Server *>::iterator s_it = _servers.begin();

    while (s_it != _servers.end())
    {
        delete *s_it;
        ++s_it;
    }
    _servers.clear();
}

void Config::parse(const std::string &path_to_file)
{
    std::string line;
    std::ifstream file(path_to_file.c_str());
    std::vector<std::string> _config;
    std::string token;

    if (!file.is_open())
    {
        throw Config::ConfigException("config file " + path_to_file + " is locked");
    }

    while (std::getline(file, line))
    {
        // skip comment and empty line
        if (!line.length() || isComment(line))
        {
            continue;
        }
        // clear comment in the end
        line = line.substr(0, line.find("#", 0));
        // split by witespace
        std::istringstream iss(line);
        std::vector<std::string> tokens(
            (std::istream_iterator<std::string>(iss)),
            std::istream_iterator<std::string>());
        std::vector<std::string>::iterator token;
        // check trash derective
        if (tokens.size() > 0)
        {
            this->checkTrashDirective(tokens[0]);
        }
        for (token = tokens.begin(); token != tokens.end(); ++token)
        {
            std::string t = *token;

            if (t.length() > 1 && isDirectiveLine(t))
            {
                // delete last chr
                _config.push_back(t.erase(t.size() - 1));
                _config.push_back(";");
            }
            else
            {
                _config.push_back(*token);
            }
        }
    }
    file.close();
    this->parseServerData(_config.begin(), _config.end());
}

void Config::parseServerData(
    std::vector<std::string>::iterator begin,
    std::vector<std::string>::iterator end)
{
    int length;
    std::string *str;

    if (begin == end)
        return ;
    if (*begin != "server" || *(++begin) != "{")
        throw Config::ConfigException("invalid number of arguments in server directive in" + _path_to_file);
    ++begin;

    Server *server = new Server();

    while (begin != end && *begin != "}")
    {
        length = sizeof(directives) / sizeof(directives[0]);
        str = std::find(directives, directives + length, *begin);

        if (
            str != directives + length &&
            str - directives < static_cast<long>(
                sizeof(parseDirective) / sizeof(parseDirective[0])))
        {
            ++begin;
            (this->*parseDirective[(int)(str - directives)])(server, begin, end);
        }
        // else
        // {
        //     throw Config::ConfigException("invalid number of arguments in server directive in " + _path_to_file);
        // }
        begin++;
    }
    _servers.push_back(server);
    if (begin != end)
        begin++;
    this->parseServerData(begin, end);
}

const std::vector<Server *> &Config::getServers() const
{
    return this->_servers;
}

Config::ConfigException::ConfigException()
: msg("")
{}

Config::ConfigException::ConfigException(const std::string &msg)
: msg("webserv: [error] " + msg)
{}

Config::ConfigException::~ConfigException() throw()
{}

const char *Config::ConfigException::what() const throw()
{
    return msg.c_str();
}

bool Config::isComment(std::string const &str) const
{
    std::size_t found = str.find_first_not_of(" \t\f\v\n\r");

    if (found != std::string::npos)
    {
        if (str.at(found) == '#')
        {
            return true;
        }
    }
    return false;
}

bool Config::isDirectiveLine(std::string const &str) const
{
    std::size_t found = str.find_last_not_of(" \t\f\v\n\r");
    
    if (found != std::string::npos)
    {
        if (str.at(found) == ';')
        {
            return true;
        }
    }
    return false;
}

void Config::checkTrashDirective(std::string const &name) const
{
    int length = sizeof(directives)/sizeof(directives[0]);
    std::string *str;

    if (name != "{" && name != "}" && name != ";")
    {

        str = std::find(directives, directives + length, name);
        if (str == directives + length)
        {
            throw Config::ConfigException(
                "unknown directive " + name + " in " + _path_to_file); 
        }
    }
}

void Config::parseListen(
    IDirective *server,
    std::vector<std::string>::iterator &value,
    std::vector<std::string>::iterator &end)
{
    if (dynamic_cast<Location *>(server))
        throw Config::ConfigException();
    ((Server *)server)->setHost(inet_addr((*value).data()));
}

void Config::parsePort(
    IDirective *server,
    std::vector<std::string>::iterator &value,
    std::vector<std::string>::iterator &end)
{
    if (dynamic_cast<Location *>(server))
        throw Config::ConfigException();
    ((Server *)server)->setPort((uint16_t) atoll((*value).c_str()));
}

void Config::parseServerName(
    IDirective *server,
    std::vector<std::string>::iterator &value,
    std::vector<std::string>::iterator &end)
{
    if (dynamic_cast<Location *>(server))
        throw Config::ConfigException();
    ((Server *)server)->setServerName(*value);
}

void Config::parseClientMaxBodySize(
    IDirective *server,
    std::vector<std::string>::iterator &value,
    std::vector<std::string>::iterator &end)
{
    if (dynamic_cast<Location *>(server))
        throw Config::ConfigException();
    ((Server *)server)->setClientMaxBodySize(atoll((*value).c_str()) * 1000000);
}

void Config::parseMimeConfPath(
    IDirective *server,
    std::vector<std::string>::iterator &value,
    std::vector<std::string>::iterator &end)
{
    if (dynamic_cast<Location *>(server))
        throw Config::ConfigException();
    ((Server *)server)->setMimeConfPath(*value);
}

void Config::parseErrorPages(
    IDirective *server,
    std::vector<std::string>::iterator &value,
    std::vector<std::string>::iterator &end)
{
    if (dynamic_cast<Location *>(server))
        throw Config::ConfigException();
    short code = (short) std::atoi((*value).c_str());
    ++value;
    ((Server *)server)->setErrorPage(std::make_pair(code, *value));
}

void Config::parseLocation(
    IDirective *server,
    std::vector<std::string>::iterator &value,
    std::vector<std::string>::iterator &end)
{
    int length;
    std::string *str;
    std::string path = *value;

    if (*(++value) != "{")
        throw Config::ConfigException(
            "invalid number of arguments in location directive in " + _path_to_file);
    ++value;

    Location *location = new Location();

    while (*value != "}")
    {
        length = sizeof(directives) / sizeof(directives[0]);
        str = std::find(directives, directives + length, *value);

        if (
            str != directives + length &&
            str - directives < static_cast<long>(
                sizeof(parseDirective) / sizeof(parseDirective[0])))
        {

            try
            {
                ++value;
                (this->*parseDirective[(int)(str - directives)])(location, value, end);
            }
            catch(...)
            {
                throw Config::ConfigException(
                    "unknown location directive " + *str  + " in " + _path_to_file);
            }
        }
        // else
        // {
        //     throw Config::ConfigException("invalid number of arguments in server directive in " + _path_to_file);
        // }
        value++;
    }
    location->setPath(path);
    ((Server *)server)->setLocation(location);
}

void Config::parseRedirection(
    IDirective *server,
    std::vector<std::string>::iterator &value,
    std::vector<std::string>::iterator &end)
{
    if (dynamic_cast<Location *>(server))
        throw Config::ConfigException();
    ((Server *)server)->setRedirection(*value);
}

void Config::parseRoot(
    IDirective *server,
    std::vector<std::string>::iterator &value,
    std::vector<std::string>::iterator &end)
{
    ((Location *)server)->setRoot(*value);
}

void Config::parseMethods(
    IDirective *server,
    std::vector<std::string>::iterator &value,
    std::vector<std::string>::iterator &end)
{
    while (*value != ";")
    {
        int length = sizeof(methods) / sizeof(methods[0]);
        std::string *str = std::find(methods, methods + length, *value);

        if (str != methods + length)
        {
            ((Location *)server)->setMethod((int)(str - methods), true);
        }
        else
        {
            throw Config::ConfigException("method not supported in " + _path_to_file);
        }
        value++;
    }
    --value;
}

void Config::parseFileUpload(
    IDirective *server,
    std::vector<std::string>::iterator &value,
    std::vector<std::string>::iterator &end)
{
    bool file_upload;

    if (*value == "on")
        file_upload = true;
    else
        file_upload = false;
    // exception
    // 
    ((Location *)server)->setFileUpload(file_upload);
}

void Config::parseUploadTmpPath(
    IDirective *server,
    std::vector<std::string>::iterator &value,
    std::vector<std::string>::iterator &end)
{
    ((Location *)server)->setUploadTmpPath(*value);
}

void Config::parseIndex(
    IDirective *server,
    std::vector<std::string>::iterator &value,
    std::vector<std::string>::iterator &end)
{
    ((Location *)server)->setIndex(*value);
}

void Config::parseAutoindex(
    IDirective *server,
    std::vector<std::string>::iterator &value,
    std::vector<std::string>::iterator &end)
{
    bool autoindex;

    if (*value == "on")
        autoindex = true;
    else
        autoindex = false;
    // exception
    // 
    ((Location *)server)->setAutoindex(autoindex);
}

void Config::parseCgiPass(
    IDirective *server,
    std::vector<std::string>::iterator &value,
    std::vector<std::string>::iterator &end)
{
    ((Location *)server)->setCgiPass(*value);
}
