/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dirituay <dirituay@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/18 19:29:45 by dianarituay       #+#    #+#             */
/*   Updated: 2026/01/19 13:09:50 by dirituay         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>
#include <ctime>

class Client
{
	private:
		int _fd; //fd del socket
		std::string _ip;
		int _port; 
		//std::string _requestBuffer; //acumula datos recibidos
		//std::string _responseBuffer; //datos pendientes de enviar
		time_t _lastActivity; 
	public:
		Client(int fd, const std::string& ip, int port);
		//getters
		int getFd() const;
		std::string getIp() const;
		int getPort() const;
		/*request buffer management
		void appendToRequest(const std::string& data); 
		bool isRequestComplete() const;
		std::string getRequest() const;
		void clearRequest();
		//reponse buffer management
		void setResponse(const std::string& reponse);
		std::string getResponse() const;
		void consumeResponse(size_t bytes);
		bool hasResponsePending() const; */
		//time
		void updateActivity();
		//bool isTimedOut(time_t now, int timeoutSeconds) const;
		
};

#endif

/*guarda la ficha de info. de 1 cliente(1 cliente en el resto)
Cliente #1:
├─ FD: 5
├─ IP: 127.0.0.1
├─ Puerto: 54321
├─ Request acumulado: "GET /index.html HTTP/1.1\r\n..."
├─ Response pendiente: "HTTP/1.1 200 OK\r\n..."
└─ Última actividad: 15:30:42
*REQUEST BUFFER* (lo q el cliente ENVIA)
es donde acumulo los datos q el cliente va enviando
*RESPONSE BUFFER* (lo q yo envio al cliente)
guarda los datos q quiero enviar pero q no se enviaron completos aun
*/