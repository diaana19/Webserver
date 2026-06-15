#ifndef PARSECONFIG_HPP
# define  PARSECONFIG_HPP

#include "Config.hpp"
#include "ServerParsing.hpp"

class ParseConfig {
	private :
		std::vector<Token> tokenizeConfigFile(const std::string &filename);
		void parseServersAndLocation(std::vector<Token> &tokens);
		std::vector<ServerConfig> allServers;
		
	public :
		ParseConfig();
		~ParseConfig();

		void start(const std::string &fileName);
		std::vector<ServerConfig> getAllServerConfig();
};

#endif