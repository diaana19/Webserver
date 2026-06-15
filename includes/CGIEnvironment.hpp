#ifndef CGIENVIRONMENT_HPP
#define CGIENVIRONMENT_HPP

#include "HttpRequest.hpp"
#include "Config.hpp"
#include "Colors.hpp"
#include <string>
#include <map>

struct CGIProcess {
    pid_t       pid;
    int         outputPipeFd;
    int         clientFd;
    std::string buffer;
    bool        readyToRead;
    time_t 		startTime;
	CGIProcess(): outputPipeFd(-1), clientFd(-1), readyToRead(-1){};
};

class CGIEnvironment
{
	private:
		std::string intToString(int n);
		void parseScriptAndPath(const std::string& fullPath, const std::vector<std::string>& cgiExtensions, std::string& scriptName, std::string& pathInfo);
	public:
		CGIEnvironment();
		~CGIEnvironment();
		char ** build(const HttpRequest& request, const LocationConfig& location, const ServerConfig& server);
};

#endif