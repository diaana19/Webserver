#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <string>
#include <vector>
#include <map>
#include "Token.hpp"

class LocationConfig
{
	public:
		std::string uri;
		std::string rootLocation;
		std::vector<std::string> index;
		std::vector<std::string> allow_methods;
		std::string uploadPath;
		int autoindex;
		int redirect_code;
		std::string redirect_target;
		std::vector<std::string> cgi_extension;
		std::string cgi_interpreter;
		std::string cgi_path;
		bool cgi_On;
		LocationConfig();
		~LocationConfig();
		LocationConfig(const LocationConfig &other);
		LocationConfig& operator=(const LocationConfig &other);
};

struct ServerConfig
{
	int port; //8080
	std::string host;
	std::string root;
	std::string serverName;
	size_t clientMaxBodySize;
	std::string index; //index.html
	std::map<int, std::string> errorPages;
	std::vector<LocationConfig> locations;

	ServerConfig() : port(0), host("0.0.0.0"), root(""), clientMaxBodySize(10485760), index("") {};
	ServerConfig(const ServerConfig& other)
    {
        port = other.port;
        host = other.host;
        root = other.root;
        serverName = other.serverName;
        clientMaxBodySize = other.clientMaxBodySize;
        index = other.index;
        errorPages = other.errorPages;
        locations = other.locations;
    };
	ServerConfig& operator=(const ServerConfig& other)
	{
	    if (this != &other)
	    {
	        port = other.port;
	        host = other.host;
	        root = other.root;
	        serverName = other.serverName;
	        clientMaxBodySize = other.clientMaxBodySize;
	        index = other.index;
	        errorPages = other.errorPages;
	        locations = other.locations;
	    }
	    return (*this);
	}
};

struct Config
{
	std::vector<ServerConfig> servers;
};

class ConfigParser {
	public:
		Config parse(const std::vector<Token> &tokens); 
};

#endif