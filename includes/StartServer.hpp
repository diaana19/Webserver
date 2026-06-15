#ifndef STARTSERVER_HPP
# define  STARTSERVER_HPP

#include "Config.hpp"
#include "ServerParsing.hpp"
#include "ClientManager.hpp"
#include "EventLoop.hpp"
#include "LocationPathTransform.hpp"

class StartServer {
	private :
		
	public :
		StartServer();
		~StartServer();

		bool start(std::vector<ServerConfig> &allServers);
};

#endif