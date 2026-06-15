#include "ClientManager.hpp"
#include "Colors.hpp"
#include <iostream>
#include <cstring> 
#include <cerrno> 
#include <netinet/in.h> 
#include <arpa/inet.h> 
#include <sys/socket.h>
#include <unistd.h>

ClientManager::ClientManager()
{
	std::cout << B_CYAN << "[SERVER]" << RESET << " ClientManager initialized" << std::endl;
}

ClientManager::~ClientManager()
{
	for (std::map<int, Client>::iterator it = _clients.begin(); it != _clients.end(); ++it)
	{
		close (it->first); 
	}
	std::cout << B_CYAN << "[SERVER]" << RESET << " ClientManager destroyed" << std::endl;
}

int ClientManager::make_socket_non_blocking(int fd) 
{
	return (fcntl(fd, F_SETFL, O_NONBLOCK));
};


int ClientManager::acceptNewClient(int serverFd)
{
	struct sockaddr_in clientAddr; 
	socklen_t clientAddrlen = sizeof(clientAddr); 

	int clientFd = accept(serverFd, (struct sockaddr*)&clientAddr, &clientAddrlen);
	if (clientFd == -1)
	{
		
        
		std::cerr << "accept: " << strerror(errno) << std::endl;
		return (-1); 
	}
	make_socket_non_blocking(clientFd);
	char clientIP[INET_ADDRSTRLEN]; 
	inet_ntop(AF_INET, &clientAddr.sin_addr, clientIP, INET_ADDRSTRLEN); 
	
	int clientPort = ntohs(clientAddr.sin_port); 
	_clients.insert(std::make_pair(clientFd, Client(clientFd, std::string(clientIP), clientPort)));

	
	std::cout << std::endl;
	std::cout << RED << "✓ New client connected:" << RESET << std::endl;
    std::cout << "  FD:   " << clientFd << std::endl;
    std::cout << "  IP:   " << clientIP << std::endl;
    std::cout << "  Port: " << clientPort << std::endl;
    std::cout << "  Total clients: " << _clients.size() << std::endl;
	
	return (clientFd);
}


void ClientManager::removeClient(int fd)
{
	std::map<int, Client>::iterator it = _clients.find(fd);
	if (it != _clients.end())
	{
		std::cout << RED << "x Client disconnected: FD " << RESET << fd << " (" << it->second.getIp() << ")" << std::endl;
		close (fd); 
		_clients.erase(it);
		std::cout << GREEN << " Clients remaining: " << RESET << _clients.size() << std::endl;
	}
}

void ClientManager::printClients() const
{
	 std::cout << "\n=== CLIENTES CONECTADOS ===" << std::endl;
    std::cout << "Total: " << _clients.size() << std::endl;
    
    for (std::map<int, Client>::const_iterator it = _clients.begin(); it != _clients.end(); ++it) {
        std::cout << "FD " << it->first 
                  << " - " << it->second.getIp() 
                  << ":" << it->second.getPort() << std::endl;
    }
    std::cout << "===========================\n" << std::endl;
}

size_t ClientManager::getClientCount() const
{
	return _clients.size();
}

Client* ClientManager::getClient(int fd)
{
	std::map<int, Client>::iterator it = _clients.find(fd);
	if (it != _clients.end())
		return &(it->second); 
	return (NULL);
}

bool ClientManager::hasClient(int fd) const
{	
	return (_clients.find(fd) != _clients.end()); 
}