/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vali <vali@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/23 22:33:08 by dianarituay       #+#    #+#             */
/*   Updated: 2026/03/08 19:52:33 by vali             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

		// Make a listing of all file in the directory
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
	std::string host; // "127.0.0.1"
	std::string root; // /var/www;
	std::string serverName; //"mi_server"
	size_t clientMaxBodySize; // 10485760(10MB en bytes)
	std::string index; //index.html
	std::map<int, std::string> errorPages; // {404: "/errors/404.html"}
	std::vector<LocationConfig> locations;

	//constrcutor por defecto
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

//server config. complete
struct Config // plusieurs serveurs
{
	std::vector<ServerConfig> servers;
};

//parser principal
class ConfigParser {
	public:
		Config parse(const std::vector<Token> &tokens); 
};
#endif