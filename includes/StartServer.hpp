/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   StartServer.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dianarituay <dianarituay@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/26 10:42:31 by dianarituay       #+#    #+#             */
/*   Updated: 2026/02/26 10:42:33 by dianarituay      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STARTSERVER_HPP
# define  STARTSERVER_HPP

#include "Config.hpp"
// #include "ConfigParser.hpp"
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