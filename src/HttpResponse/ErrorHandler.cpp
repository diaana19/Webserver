/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ErrorHandler.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dianarituay <dianarituay@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/26 10:43:35 by dianarituay       #+#    #+#             */
/*   Updated: 2026/02/26 10:43:36 by dianarituay      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ErrorHandler.hpp"

std::string ErrorHandler::convertNumberIntoString(int nb) {
	std::ostringstream oss;
	oss << nb;
	std::string lineNb = oss.str();
	return (lineNb);
}
// taken from HeaderBuilder
ErrorHandler::ErrorHandler(){
    constructMap();
}

ErrorHandler::~ErrorHandler(){
}

void ErrorHandler::constructMap()
{
    errorString[409] = "Conflict";
    errorString[410] = "Gone";
    errorString[411] = "Length Required";

        // Informational 
    errorString[100] = "Continue";
    errorString[101] = "Switching Protocols";
    
    // Success 
    errorString[200] = "OK";
    errorString[201] = "Created";
    errorString[202] = "Accepted";
    errorString[204] = "No Content";
    errorString[206] = "Partial Content";
    
    //  Redirection 
    errorString[300] = "Multiple Choices";
    errorString[301] = "Moved Permanently";
    errorString[302] = "Found";
    errorString[303] = "See Other";
    errorString[304] = "Not Modified";
    errorString[307] = "Temporary Redirect";
    errorString[308] = "Permanent Redirect";
    
    // Client Error 
    errorString[400] = "Bad Request";
    errorString[401] = "Unauthorized";
    errorString[403] = "Forbidden";
    errorString[404] = "Not Found";
    errorString[405] = "Method Not Allowed";
    errorString[408] = "Request Timeout";
    errorString[409] = "Conflict";
    errorString[410] = "Gone";
    errorString[411] = "Length Required";
    errorString[413] = "Payload Too Large";
    errorString[414] = "URI Too Long";
    errorString[415] = "Unsupported Media Type";
    errorString[418] = "I'm a teapot"; 
    errorString[429] = "Too Many Requests";
    
    // Server Error 
    errorString[500] = "Internal Server Error";
    errorString[501] = "Not Implemented";
    errorString[502] = "Bad Gateway";
    errorString[503] = "Service Unavailable";
    errorString[504] = "Gateway Timeout";
    errorString[505] = "HTTP Version Not Supported";
}
std::string ErrorHandler::readFile(std::string &pathfile) {
	std::ifstream fileIS(pathfile.c_str(), std::ios::binary);
	if (!(fileIS.is_open())) {
		return ("");
	}
	std::ostringstream buffer;
	buffer << fileIS.rdbuf();
	buffer << "\r\n";
	return (buffer.str());
};

std::string ErrorHandler::defaultErrorPage(int errorCode){
    std::map<int, std::string>::iterator it  = errorString.find(errorCode);
    
    if(it == errorString.end())
        return ("Unknow");
    if(errorCode == 204) // No content
        return ("");
    std::string body;
    std::string strCode = convertNumberIntoString(errorCode);
    // body += "<html>\n<head>\n<title>" + strCode + " " + it->second + "</title>\n</head>\r\n";
	// body += "<body style=\"font-family:sans-serif;\">\n<center><h1>" + strCode + " " + it->second + "</h1></center><hr/>\r\n";
	// body += "<center>webserv</center>\r\n";
	// body += "</body>\n</html>\r\n";
	// body += "\r\n";
    body += "<html>\n<head>\n<title>" + strCode + " " + it->second + "</title>\n</head>\r\n";
	body += "<body style=\"font-family:sans-serif;\">\n<center><h1>" + strCode + " " + it->second + "</h1></center><hr/>\r\n";
    body += "\r\n";
    return (body);
};

std::string ErrorHandler::generate(int errorCode, const ServerConfig &serverConfig){
    std::string pathErrorPage = searchErrorCodePage(errorCode, serverConfig);
    std::string body;
    std::ifstream file(pathErrorPage.c_str());
    if (file.is_open()) {
        body = readFile(pathErrorPage);
    } else {
        body = defaultErrorPage(errorCode);
    }
    return (body);
};
