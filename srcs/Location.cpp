#include "Location.hpp"

// default
Location::Location(): 
	_path(""),
	_root(""),
	_methods(3, false),
	_file_upload(false),
	_upload_tmp_path(""),
	_index(""),
	_autoindex(false),
	_cgi_pass(""),
	_redirection("")
{
	// default GET
	_methods[0] = true;
}

Location::~Location()
{}

std::string Location::getPath() const
{
	return _path;
}

std::string Location::getRoot() const
{
	return _root;
}

std::vector<bool> Location::getMethods() const
{
	return _methods;
}

bool Location::getFileUpload() const
{
	return _file_upload;
}

std::string Location::getUploadTmpPath() const
{
	return _upload_tmp_path;
}

std::string Location::getIndex() const
{
	return _index;
}

bool Location::getAutoindex() const
{
	return _autoindex;
}

std::string Location::getCgiPass() const
{
	return _cgi_pass;
}

std::string Location::getRedirection() const
{
    return _redirection;
}

void Location::setPath(std::string const &path)
{
	_path = path;
}

void Location::setRoot(std::string const &root)
{
	_root = root;
}

void Location::setMethods(std::vector<bool> methods)
{
	_methods = methods;
}

void Location::setMethod(bool method)
{
	_methods.push_back(method);
}

void Location::setMethod(unsigned int index, bool method)
{
	_methods[index] = method;
}

void Location::setFileUpload(bool file_upload)
{
	_file_upload = file_upload;
}

void Location::setUploadTmpPath(std::string const &upload_tmp_path)
{
	_upload_tmp_path = upload_tmp_path;
}

void Location::setIndex(std::string const &index)
{
	_index = index;
}

void Location::setAutoindex(bool autoindex)
{
	_autoindex = autoindex;
}

void Location::setCgiPass(std::string const &cgi_pass)
{
	_cgi_pass = cgi_pass;
}

void Location::setRedirection(std::string const &redirection)
{
    _redirection = redirection;
}
