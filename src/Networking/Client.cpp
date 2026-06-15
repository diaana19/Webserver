#include "Client.hpp"
#include <iostream>

Client::Client(int fd, const std::string& ip, int port) : _fd(fd), _ip(ip), _port(port), _lastActivity(time(NULL))
{
	
}

int Client::getFd() const
{
	return(_fd);
}

std::string Client::getIp() const
{
	return (_ip);
}

int Client::getPort() const
{
	return (_port);
}