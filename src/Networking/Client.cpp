/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vali <vali@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/18 19:30:39 by dianarituay       #+#    #+#             */
/*   Updated: 2026/01/19 19:42:18 by vali             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"
#include <iostream>

Client::Client(int fd, const std::string& ip, int port) : _fd(fd), _ip(ip), _port(port), _lastActivity(time(NULL))
{
	//_requestBuffer("");
	//_responseBuffer("");
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

/*agrega + datos al buffer pq los datos llegan por partes
void Client::appendToRequest(const std::string &data)
{
	
}

// verifica si ya recibimos el request completo => termina con \r\n\r\n (2 saltos de linea)
bool Client::isRequestComplete() const
{
	
}

//devuelve el contenido completo del buffer de request
std::string Client::getRequest() const
{
	
}

//limpia el buffer pq HTTP puede enviar multiples requests en la misma conexion 
void Client::clearRequest()
{
	
}

//establece la rpta HTTP completa q enviaremos 
void Client::setResponse(const std::string &reponse)
{
	
}

//devuelve el contenido del buffer de respuesta
std::string Client::getResponse() const
{
	
}

//elimina los N bytes de respuesta q ya se enviaron pq send() no puede enviar todo en 1 sola vez
void Client::consumeResponse(size_t bytes)
{
	
}

//verifica si aun hay datos pendientes a enviar
bool Client::hasResponsePending() const
{
	
}

//
void Client::updateActivity()
{
	
}

//
bool Client::isTimedOut(time_t now, int timeoutSeconds) const
{
	
}*/
