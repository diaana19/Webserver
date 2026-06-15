#pragma once
#ifndef ERRORHANDLER_HPP
# define  ERRORHANDLER_HPP

#include <string>
#include <unistd.h>
#include <fstream>
#include "Config.hpp"

class ErrorHandler {
	private :
        std::map<int, std::string> errorString;
		std::string searchErrorCodePage(int errorCode, const ServerConfig &serverConfig) {
			std::map<int, std::string>::const_iterator it = serverConfig.errorPages.find(errorCode);
			if (it != serverConfig.errorPages.end()) {
				return (it->second);
			}
			std::string nothing;
			return (nothing);
		};
			std::string readFile(std::string &pathfile);
			std::string convertNumberIntoString(int nb);
			std::string defaultErrorPage(int errorCode);
		
	public :
		ErrorHandler();
		~ErrorHandler();
		std::string generate(int errorCode, const ServerConfig &serverConfig);
};

#endif