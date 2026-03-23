/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vali <vali@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/19 12:43:10 by dirituay          #+#    #+#             */
/*   Updated: 2026/03/11 00:38:33 by vali             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

 #include <iostream>
 #include <cstring>
 #include <cerrno>
 #include <cstdio>
 #include <unistd.h>
 #include <netinet/in.h>
 #include <netdb.h>
 #include <sys/socket.h>
 #include <fstream>
 #include <sstream>
 #include <string>
 #include <sys/stat.h>
 #include <fcntl.h>
 #include <arpa/inet.h>
#include <map>

class Server
{
	private:
		long	_fd;
		sockaddr_in _addr;
	public:
		Server();
		Server(const Server& copy);
		Server& operator =(const Server& assign);
		~Server();
		int setupSocket(std::string ipAdress, int port);
		void setupAddress(std::string ipAdress, int port);
		int getFd() const;
		sockaddr_in get_addr() const {return (this->_addr);};

};