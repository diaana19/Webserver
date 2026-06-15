#include "StatusBuilder.hpp"
#include <sstream>

StatusBuilder :: StatusBuilder()
{
    statusMsgInit();
}

StatusBuilder :: ~StatusBuilder()
{}

void StatusBuilder :: statusMsgInit()
{
    _statusMsg[409] = "Conflict";
    _statusMsg[410] = "Gone";
    _statusMsg[411] = "Length Required";

        
    _statusMsg[100] = "Continue";
    _statusMsg[101] = "Switching Protocols";
    
    
    _statusMsg[200] = "OK";
    _statusMsg[201] = "Created";
    _statusMsg[202] = "Accepted";
    _statusMsg[204] = "No Content";
    _statusMsg[206] = "Partial Content";
    
    
    _statusMsg[300] = "Multiple Choices";
    _statusMsg[301] = "Moved Permanently";
    _statusMsg[302] = "Found";
    _statusMsg[303] = "See Other";
    _statusMsg[304] = "Not Modified";
    _statusMsg[307] = "Temporary Redirect";
    _statusMsg[308] = "Permanent Redirect";
    
    
    _statusMsg[400] = "Bad Request";
    _statusMsg[401] = "Unauthorized";
    _statusMsg[403] = "Forbidden";
    _statusMsg[404] = "Not Found";
    _statusMsg[405] = "Method Not Allowed";
    _statusMsg[408] = "Request Timeout";
    _statusMsg[409] = "Conflict";
    _statusMsg[410] = "Gone";
    _statusMsg[411] = "Length Required";
    _statusMsg[413] = "Payload Too Large";
    _statusMsg[414] = "URI Too Long";
    _statusMsg[415] = "Unsupported Media Type";
    _statusMsg[418] = "I'm a teapot"; 
    _statusMsg[429] = "Too Many Requests";
    
    
    _statusMsg[500] = "Internal Server Error";
    _statusMsg[501] = "Not Implemented";
    _statusMsg[502] = "Bad Gateway";
    _statusMsg[503] = "Service Unavailable";
    _statusMsg[504] = "Gateway Timeout";
    _statusMsg[505] = "HTTP Version Not Supported";
}

std::string	StatusBuilder :: getStatusMsg(int code)
{
    std::map<int, std::string>::iterator it  = _statusMsg.find(code);

    
    if(it != _statusMsg.end())
        return it->second;

    if(code >= 100 && code < 200)
        return "Informational";
    if(code >= 200 && code < 300)
        return "Success";
    if(code >= 300 && code < 400)
        return "Redirection";
    if(code >= 400 && code < 500)
        return "Client Error";
    if(code >= 500 && code < 600)
        return "Server Error";

    return "Unknown";

}

std::string StatusBuilder::build(int code)
{
    return build(code, "HTTP/1.1");
}

std::string StatusBuilder::build(int code, const std::string &httpVersion)
{
    std::ostringstream oss;

    oss << httpVersion << " " << code << " " << getStatusMsg(code) << "\r\n";

    return oss.str();
}