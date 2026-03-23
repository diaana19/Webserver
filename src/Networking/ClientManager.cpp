/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientManager.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dianarituay <dianarituay@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/18 19:30:53 by dianarituay       #+#    #+#             */
/*   Updated: 2026/01/23 17:25:20 by dianarituay      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ClientManager.hpp"
#include "Colors.hpp"
#include <iostream>
#include <cstring> //strerror
#include <cerrno> //errno
#include <netinet/in.h> // inet_addstrlen
#include <arpa/inet.h> // inet_ntop
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
		close (it->first); // cada vez q avanza va cerrando los (fds)
	}
	std::cout << B_CYAN << "[SERVER]" << RESET << " ClientManager destroyed" << std::endl;
}

int ClientManager::make_socket_non_blocking(int fd) 
{
	return (fcntl(fd, F_SETFL, O_NONBLOCK));
};

//guarda el nuevo cliente, accept()
int ClientManager::acceptNewClient(int serverFd)
{
	struct sockaddr_in clientAddr; //struc para almacenar la info del cliente
	socklen_t clientAddrlen = sizeof(clientAddr); // accept() tiene q saber la size de la struct

	int clientFd = accept(serverFd, (struct sockaddr*)&clientAddr, &clientAddrlen);
	if (clientFd == -1)
	{
		// if (errno == EAGAIN || errno == EWOULDBLOCK)
        //    return (-1); //si no hay clientes, try again
		std::cerr << "accept: " << strerror(errno) << std::endl;
		return (-1); 
	}
	make_socket_non_blocking(clientFd);
	char clientIP[INET_ADDRSTRLEN]; //como clientIp[16] pq IPv4 necesita 16 bytes
	inet_ntop(AF_INET, &clientAddr.sin_addr, clientIP, INET_ADDRSTRLEN); //IP en binario
	
	int clientPort = ntohs(clientAddr.sin_port); //convierte la port al formato del CPU
	_clients.insert(std::make_pair(clientFd, Client(clientFd, std::string(clientIP), clientPort)));

	//debug
	std::cout << std::endl;
	std::cout << RED << "✓ New client connected:" << RESET << std::endl;
    std::cout << "  FD:   " << clientFd << std::endl;
    std::cout << "  IP:   " << clientIP << std::endl;
    std::cout << "  Port: " << clientPort << std::endl;
    std::cout << "  Total clients: " << _clients.size() << std::endl;
	
	return (clientFd);
}

//limpia el cliente desconectado
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

//busca un cliente por su fd y devuelve un puntero a ese cliente
Client* ClientManager::getClient(int fd)
{
	std::map<int, Client>::iterator it = _clients.find(fd);
	if (it != _clients.end())
		return &(it->second); //retorna el  puntero al cliente
	return (NULL);
}

//verifica si un fd esta en el map de clientes
bool ClientManager::hasClient(int fd) const
{	
	return (_clients.find(fd) != _clients.end()); // si existe o no en el map
}


/* => cast a (struct sockaddr*) pq accept() espera un puntero a struct sockaddr
pero puse struct sockadd_in (q es especifico para IPv4)
*/