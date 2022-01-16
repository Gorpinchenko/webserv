#include "MimeType.hpp"

MimeType::MimeType(const std::string &path_to_conf) {
    std::list<std::string> res;

    MimeType::tokenizeFileStream(path_to_conf, res); //TODO разобраться
    std::list<std::string>::iterator it = res.begin();
    std::list<std::string>::iterator end = res.end();

    if (*it != "types") {
        throw MimeTypeException("Mime config error");
    } else {
        skipTokens(it, end, 2);
    }

    for (; it != end; ++it) {
        if (*it == "}") {
            std::cout << "111111111111111";
            break;
        }
        std::string mime_type;
        mime_type = *it;
        skipTokens(it, end, 1);
        std::cout << mime_type << " : " << *it << std::endl; //TODO убрать

        while (*it->rbegin() != ';') {
//            _types.insert(std::make_pair(*it, mime_type));
            skipTokens(it, end, 1);
        }
        if (*(it->rbegin()) != ';') {
            throw MimeTypeException("Mime confing syntax error");
        }
    }
    if (*(it->rbegin()) != '}') {
        throw MimeTypeException("Mime confing syntax error1");
    }
    std::list<std::string>::iterator test = ++it;
    if (!(test->empty()))  {
        throw MimeTypeException("Mime confing syntax error1111");
    }
}

void MimeType::skipTokens(std::list<std::string>::iterator &it,
                       std::list<std::string>::iterator &end, int num) {
    for (int i = 0; i < num; ++i) {
        if (it == end) {
            throw MimeTypeException("Token syntax error");
        }
        ++it;
    }
    if (it == end) {
        throw MimeTypeException("Token syntax error");
    }
}

void MimeType::tokenizeFileStream(std::string const &file_path, std::list<std::string> &res) {
    std::string   token;
    std::ifstream is(file_path, std::ifstream::binary);
    std::string   line;

    if ((is.rdstate() & std::ifstream::failbit) != 0) {
        throw MimeTypeException("Error opening " + file_path + "\n");
    }
    if (is) {
        while (std::getline(is, line)) {
            token.clear();
            for (std::string::iterator it = line.begin(); it != line.end(); ++it) {
                if (std::isspace(*it) || *it == ';') {
                    if (!token.empty()) {
                        res.push_back(std::string(token));
                        token.clear();
                    }
                    if (*it == ';') {
                        res.push_back(std::string(1, *it));
                    }
                    continue;
                }
                token.push_back(*it);
            }
            if (!token.empty()) {
                res.push_back(std::string(token));
            }
        }
    } else {
        is.close();
        throw MimeTypeException("Error EOF was not reached " + file_path + "\n");
    }
    is.close();
}
