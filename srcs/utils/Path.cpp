#include "Path.hpp"

bool Path::isDirectory(const std::string &path) {
    struct stat s = {};
    if (stat(path.data(), &s) == 0) {
        return (S_ISDIR(s.st_mode));
    }
    return (false);
}

std::string Path::getAutoIndexHtml(const std::string &path, const std::string &uri_path) {
    DIR           *dp;
    struct dirent *di_struct;
    int           i = 0;
    std::string   table;

    dp = opendir(path.data());
    table += "<h1>" + uri_path + "</h1>";
    table += "<table>";
    table += "<tr> <th>File name</th> <th>File size</th> <th>Last modified</th> </tr>";
    if (dp != nullptr) {
        while ((di_struct = readdir(dp)) != nullptr) {
            FileStats file(path + "/" + di_struct->d_name);
            table += "<tr>";

            table += "<td><a href=\"" + uri_path;
            table += di_struct->d_name;
            if (file.isDir())
                table += "/";
            table += "\">" + std::string(di_struct->d_name) + "</a></td>";
            table += "<td>" + file.getSizeInMb() + "</td>";
            table += "<td>" + file.getTimeModified() + "</td>";
            table += "</tr>";
            i++;
        }
        closedir(dp);
    }
    table += "</table>";

    return table;
//    insertTableIntoBody(table, uri_path);
}

const std::string Path::AUTOINDEX_HTML = "<!DOCTYPE html><html lang=\"en\"><head><meta charset=\"UTF-8\"><title></title></head><style>table {border: 1px solid #ccc;background-color: #f8f8f8;border-collapse: collapse;margin: 0;padding: 0;width: 100%;table-layout: fixed;text-align: left;}table td:last-child {border-bottom: 0;}</style><body></body></html>";

