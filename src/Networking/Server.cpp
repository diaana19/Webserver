/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dirituay <dirituay@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/19 12:42:39 by dirituay          #+#    #+#             */
/*   Updated: 2026/03/15 15:04:26 by dirituay         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "ClientManager.hpp"

Server :: Server():_fd(-1)
{}

Server :: Server(const Server& copy)
{
	 _fd = copy._fd;
    _addr = copy._addr;
    const_cast<Server&>(copy)._fd = -1;
}

Server& Server::operator = (const Server &assign)
{
	if (this != &assign) {
        _fd = assign._fd;
        _addr = assign._addr;
        const_cast<Server&>(assign)._fd = -1;
    }
    return (*this);
}

int Server :: setupSocket(std::string ipAdress, int port)
{
	_fd = socket(AF_INET, SOCK_STREAM, 0);
	if(_fd < 0)
	{
		perror("socket");
		return(1);
	}

    if (fcntl(_fd, F_SETFL, O_NONBLOCK) < 0)
	{
        perror("fcntl");
        close(_fd);
        return(1);
    }
    
    int opt = 1;
    if (setsockopt(_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
	{
        perror("setsockopt");
        close(_fd);
        return(1);
    }
	setupAddress(ipAdress, port);
	if(bind(_fd, (struct sockaddr*)&_addr, sizeof(_addr)) < 0)
	{
		perror("bind");
		close(_fd);
		return(1);
	}
	if(listen(_fd, 10) < 0)
	{
		perror("listen");
		close(_fd);
		return 1;
	}
	return 0;
	
}

void Server :: setupAddress(std::string ipAdress, int port)
{
	//struct sockaddr_in : Structure describing an Internet socket address.
	//INADDR para aceptar los mensajes
	std::memset(&_addr, 0, sizeof(_addr));
	// _addr.sin_family = AF_INET;
	// _addr.sin_addr.s_addr = INADDR_ANY;
	// //Manejo del puerto, !!!!CAMBIAR A FUNCION DE MANEJO DE VARIOS PUERTOS
	// _addr.sin_port = htons(8080);

	_addr.sin_family = AF_INET;
	_addr.sin_addr.s_addr = inet_addr(ipAdress.c_str());
	_addr.sin_port = htons(port);
}

//This part could be manage in client and the std::map too
// long Server :: accept()
// {
// 	int _fdClient = ::accept(_fd, NULL, NULL);
// 	if(_fdClient < 0)
// 	{
// 		perror("accept");
// 		return -1;
// 	}
// 	Client	client(_fdClient);
// 	_clients.insert(std::make_pair(_fdClient, client));
// 	return (_fdClient);
// }

int Server :: getFd() const
{
	return _fd;
}

Server :: ~Server()
{
	if (_fd != -1)
		close(_fd);
}
