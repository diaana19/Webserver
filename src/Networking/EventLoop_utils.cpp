/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EventLoop_utils.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vali <vali@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/19 13:16:13 by dirituay          #+#    #+#             */
/*   Updated: 2026/03/17 20:44:39 by vali             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "EventLoop.hpp"

std::string getFirstLine(std::string &fullRequest){
	std::istringstream streamBody(fullRequest);
	std::string line;
	getline(streamBody, line);
	return (line);
};

Server *EventLoop::findServer_byFdClient(int fd)
{
	for (std::vector<Server>::iterator it = allServers.begin(); it != allServers.end(); ++it)
	{
		if (fd == it->getFd())
			return &(*it);
	}
	return NULL;
};

size_t stringToSizeT(std::string &str)
{
    std::stringstream sstream(str);
    size_t value;
    sstream >> value;
    return (value);
};

std::pair<std::string, int> EventLoop::getHostAndPort(std::string &host)
{
    std::string ip;
    int port = 0;
	
    if (host.empty())
		return std::make_pair(std::string("127.0.0.1"), 80);
    size_t pos = host.find(':');
    if (pos == std::string::npos)
        return std::make_pair(std::string("127.0.0.1"), 80);

    ip = host.substr(0, pos);

    if (ip == "localhost")
        ip = "127.0.0.1";

    std::string portStr = host.substr(pos + 1);

    if (portStr.empty())
        return std::make_pair(ip, 80);

    port = std::atoi(portStr.c_str());

    if (port <= 0 || port > 65535)
        port = 80;

    return std::make_pair(ip, port);
};

int EventLoop::searchIndexServer(std::string ip, int port) {
	for (std::vector<ServerConfig>::iterator it = this->allServersConfig.begin(); it != this->allServersConfig.end(); it++) {
		if (it->host == ip && it->port == port)
			return (std::distance(this->allServersConfig.begin(), it));
	}
	return (-1);
};

int EventLoop::searchIndexLocation(ServerConfig &serverConfig, std::string &path) {
    int bestMatch = -1;
    size_t bestMatchLength = 0;
    
    for (size_t i = 0; i < serverConfig.locations.size(); i++) {
        std::string locationUri = serverConfig.locations[i].uri;
        
        // Match por prefijo (longest prefix match)
        if (path.compare(0, locationUri.length(), locationUri) == 0) {
            if (locationUri.length() > bestMatchLength) {
                bestMatch = i;
                bestMatchLength = locationUri.length();
            }
        }
    }
    
    return (bestMatch);
};

LocationConfig EventLoop::searchLocationConfig(HttpRequest &httpRequest) {
	(void)httpRequest;
	LocationConfig aaa;
	std::map<std::string, std::string>::iterator it = httpRequest.headers.find("Host");
	if (it != httpRequest.headers.end()) {
		std::pair<std::string, int> ipAndPort = getHostAndPort(it->second);
		int indexServer = searchIndexServer(ipAndPort.first, ipAndPort.second);
		if (indexServer != -1) {
			int indexLocation = searchIndexLocation(this->allServersConfig[indexServer], httpRequest.path);
			if (indexLocation != -1)
				return (this->allServersConfig[indexServer].locations[indexLocation]);
		}
	}
	return (aaa);
};

bool EventLoop::shouldCloseConnection(const HttpRequest& req)
{
    std::map<std::string, std::string>::const_iterator itConnect = req.headers.find("Connection");

    if (itConnect != req.headers.end()) {
        if (itConnect->second == "keep-alive")
            return (false);
        if (itConnect->second == "close")
            return (true);
    }
    if (req.httpVersion == "HTTP/1.1")
        return (false);
    return (true);
}