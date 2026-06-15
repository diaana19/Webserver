#ifndef CLIENTMANAGER_HPP
#define CLIENTMANAGER_HPP

#include "Client.hpp"
#include <map>
#include <fcntl.h>

class ClientManager
{
	private:
			std::map<int, Client> _clients;
	public:
		ClientManager();
		~ClientManager();
		int acceptNewClient(int serverFd);
		void removeClient(int fd);
		Client* getClient(int fd);
		bool hasClient(int fd) const;
		int make_socket_non_blocking(int fd);
		//debug
		void printClients() const;
		size_t getClientCount() const;
};

#endif