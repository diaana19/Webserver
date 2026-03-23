/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientManager.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dianarituay <dianarituay@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/18 19:30:05 by dianarituay       #+#    #+#             */
/*   Updated: 2026/01/23 17:24:35 by dianarituay      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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


/*es una lista de todos los clientes conectados(lista de reservas del resto)
ClientManager:
├─ Cliente con FD 5 (127.0.0.1:54321)
├─ Cliente con FD 6 (192.168.1.10:12345)
├─ Cliente con FD 7 (127.0.0.1:60000)
└─ Cliente con FD 9 (10.0.0.5:33333)
=>acepta nuevos clientes
=>almacena todos los clientes en std::map
=>elimina clientes desconectados
=>da acceso a la info. de cualquier cliente
*/