#ifndef HTTPREQUEST_HPP
# define HTTPREQUEST_HPP

# include <map>
# include <vector>
# include <string>
# include "File.hpp"
# include "Config.hpp"

struct	HttpRequest
{
    std::string method;
    std::string path;
    std::string queryString;
    std::string httpVersion;

	std::map<std::string, std::string> headers;
	std::vector<File> allFiles;
	ServerConfig serverConfig;
	LocationConfig location;
	std::string body;
	std::string clientIP;
	std::map <std::string, std::string> cookies;

	HttpRequest() :
	method(""),
	path(""),
	queryString(""),
	httpVersion(""),
	body(""),
	clientIP("127.0.0.1")
	{}
};

#endif