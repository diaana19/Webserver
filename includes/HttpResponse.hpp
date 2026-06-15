#ifndef HTTPRESPONSE_HPP
#define HTTPRESPONSE_HPP

#include <map>
#include <string>
#include <vector>
#include "CGIEnvironment.hpp"

struct HttpResponse
{
	int statusCode;

	std::map<std::string, std::string> headers;
	std::vector<std::string> setCookieHeaders;
	std::string body; 
	CGIProcess cgiProcess;
	
	HttpResponse() :
		statusCode(200),
		body("")
	{}
};

#endif