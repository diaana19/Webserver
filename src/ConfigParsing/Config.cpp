#include "Config.hpp"

LocationConfig::LocationConfig(){
    autoindex = 0;
    redirect_code = -1;
    cgi_On = false;
};
LocationConfig::~LocationConfig(){};
LocationConfig::LocationConfig(const LocationConfig &other) :
	uri(other.uri),
    rootLocation(other.rootLocation),
    index(other.index),
    allow_methods(other.allow_methods),
    uploadPath(other.uploadPath),
    autoindex(other.autoindex),
    redirect_code(other.redirect_code),
    redirect_target(other.redirect_target),
    cgi_extension(other.cgi_extension),
    cgi_interpreter(other.cgi_interpreter),
    cgi_path(other.cgi_path),
    cgi_On(other.cgi_On){
};  

LocationConfig& LocationConfig::operator=(const LocationConfig &other)
{
    if (this != &other)
    {
        uri = other.uri;
        rootLocation = other.rootLocation;
        index = other.index;
        allow_methods = other.allow_methods;
        uploadPath = other.uploadPath;
        autoindex = other.autoindex;
        redirect_code = other.redirect_code;
        redirect_target = other.redirect_target;
        cgi_extension = other.cgi_extension;
        cgi_interpreter = other.cgi_interpreter;
        cgi_path = other.cgi_path;
        cgi_On = other.cgi_On;
    }
    return *this;
}