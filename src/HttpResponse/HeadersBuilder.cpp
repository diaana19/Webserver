#include "HeadersBuilder.hpp"
#include <sstream>
#include <ctime>

HeadersBuilder::HeadersBuilder()
{
	
}

HeadersBuilder::~HeadersBuilder()
{

}


std::string HeadersBuilder::getServerHeader()
{
	return ("Server: webserver/1.0\r\n"); 
}

std::string HeadersBuilder::intToString(int n)
{
	std::stringstream ss;
	ss << n;
	return (ss.str());
}

std::string HeadersBuilder::getDateHeader()
{
	time_t now = time(NULL); 
	struct tm* gmt = gmtime(&now); 
	char buffer[100];
	strftime(buffer, sizeof(buffer), "Date: %a, %d %b %Y %H:%M:%S GMT\r\n", gmt);
	return (std::string(buffer));
}

std::string HeadersBuilder::build(const HttpResponse& response)
{
	std::string headers;
	
	for (std::map<std::string, std::string>::const_iterator it = response.headers.begin(); it != response.headers.end(); ++it)
	{
		headers += it->first + ": " + it->second + "\r\n"; 
	}
	
	headers += "Content-Length: " + intToString(response.body.size()) + "\r\n";
	
	headers += "Connection: close\r\n";
	headers += getServerHeader();
	headers += getDateHeader();
	for (size_t i = 0; i < response.setCookieHeaders.size(); i++)
	{
		headers += response.setCookieHeaders[i];
	}
	headers += "\r\n";
	return (headers);
}