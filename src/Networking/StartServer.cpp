#include "StartServer.hpp"

StartServer::StartServer() {

};

StartServer::~StartServer() {

};

bool StartServer::start(std::vector<ServerConfig> &allServers) {
	EventLoop loop;
    loop.getAllServers().reserve(allServers.size());
    for (size_t i = 0; i < allServers.size(); i++) {
        Server server;
        if (server.setupSocket(allServers[i].host, allServers[i].port) != 0)
        {
            std::cerr << "Error al crear servidor" << std::endl;
            return (false);
        }
        loop.getAllServers().push_back(server);
	}
	ClientManager clientManager;
	
	loop.addClientManager(&clientManager);
	LocationPathTransform pathTransform;
	pathTransform.transform(allServers);
	if(pathTransform.checkDuppLoc_Iterate(allServers))
		return (false);
	loop.setServersConfig(allServers);
	loop.run();
	return (true);
};
