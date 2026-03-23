/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HeadersBuilder.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dianarituay <dianarituay@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/01 17:33:16 by dianarituay       #+#    #+#             */
/*   Updated: 2026/03/19 12:31:10 by vali             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HeadersBuilder.hpp"
#include <sstream>
#include <ctime>

HeadersBuilder::HeadersBuilder()
{
	
}

HeadersBuilder::~HeadersBuilder()
{

}

//devuelve el server q identifica q software es
std::string HeadersBuilder::getServerHeader()
{
	return ("Server: webserver/1.0\r\n"); //automatico
}

std::string HeadersBuilder::intToString(int n)
{
	std::stringstream ss;
	ss << n;
	return (ss.str());
}

std::string HeadersBuilder::getDateHeader()
{
	time_t now = time(NULL); //tiempo actual
	struct tm* gmt = gmtime(&now); // convierte a GMT
	char buffer[100];
	strftime(buffer, sizeof(buffer), "Date: %a, %d %b %Y %H:%M:%S GMT\r\n", gmt);
	return (std::string(buffer));
}

std::string HeadersBuilder::build(const HttpResponse& response)
{
	std::string headers;
	
	for (std::map<std::string, std::string>::const_iterator it = response.headers.begin(); it != response.headers.end(); ++it)
	{
		headers += it->first + ": " + it->second + "\r\n"; //accedemos al nombre del header y al valor
	}
	//content-length automatico
	headers += "Content-Length: " + intToString(response.body.size()) + "\r\n";
	
	headers += "Connection: close\r\n";
	headers += getServerHeader();
	headers += getDateHeader();
	for (size_t i = 0; i < response.setCookieHeaders.size(); i++)
	{
		headers += response.setCookieHeaders[i];
	}
	headers += "\r\n";
	return (headers);
}