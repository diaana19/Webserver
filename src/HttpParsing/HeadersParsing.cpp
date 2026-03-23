/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HeadersParsing.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vali <vali@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/30 11:33:50 by dirituay          #+#    #+#             */
/*   Updated: 2026/03/19 12:50:34 by vali             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HeadersParsing.hpp"
#include "CookieParser.hpp"
#include <stdexcept>
#include <string>

HeadersParsing::HeadersParsing()
{
}

HeadersParsing::~HeadersParsing()
{
}
std::string HeadersParsing::trim(const std::string &str)
{
	size_t	start;
	size_t	end;

	if (str.empty())
		return (str);
	// encontrar primer carácter que NO sea espacio/tab
	start = 0;
	while (start < str.size() && (str[start] == ' ' || str[start] == '\t'))
		start++;
	// encontrar último carácter que NO sea espacio/tab
	end = str.size();
	while (end > start && (str[end - 1] == ' ' || str[end - 1] == '\t'))
		end--;
	return (str.substr(start, end - start));
}

// encuentra el fin del header
size_t HeadersParsing::findHeadersEnd(const std::string &rawRequest)
{
	size_t pos = rawRequest.find("\r\n\r\n"); // doble salto de linea
	if (pos == std::string::npos)
		throw std::runtime_error("Error: hearders end not found");
	return (pos);
}

// parsea cada linea del header
void HeadersParsing::parseHeaderLine(const std::string &line, HttpRequest &request)
{
	size_t	colonPos;

	if (line.empty())
		return ;
	colonPos = line.find(":");
	if (colonPos == std::string::npos)
		return ; // si no hay : es linea invalida
	std::string name = trim(line.substr(0, colonPos));
	std::string value;
	if (colonPos + 1 < line.size()) //verifica q no este fuera de rango
		value = trim(line.substr(colonPos + 1));
	else
		value = ""; //header sin valor
	request.headers[name] = value; // guardar en el map
}

// verifica si las lineas son continuacion o no
bool HeadersParsing::isMultiLineHeader(const std::string &line)
{
	if (line.empty())
		return (false);
	if (line[0] == ' ' || line[0] == '\t')
		return (true);
	return (false);
}

void HeadersParsing::parse(const std::string &rawRequest, HttpRequest &request)
{
	size_t firstLineEnd = rawRequest.find("\r\n");
	if (firstLineEnd == std::string::npos)
		return ;
	size_t headersStart = firstLineEnd + 2; //pos = 29 donde empieza H => host
	size_t headersEnd = findHeadersEnd(rawRequest);
	std::string headersSection = rawRequest.substr(headersStart, headersEnd - headersStart); //extraer la seccion de headers
	size_t pos = 0; //para separar en lineas individuales
	std::string lastHeaderName = ""; //para multi-linea
	while (pos < headersSection.size())
	{
		size_t lineEnd = headersSection.find("\r\n", pos);
		if (lineEnd == std::string::npos)
			lineEnd = headersSection.size();
		std::string line = headersSection.substr(pos, lineEnd - pos); //extraer linea sin el \r\n
		// std::cout << "line:" << line << std::endl;
		if (line.empty())
			break ;	
		if (isMultiLineHeader(line))
		{
			if (!lastHeaderName.empty())
				request.headers[lastHeaderName] += " " + trim(line); //continuacion, espacio y linea limpia agregada
		}
		else
		{
				parseHeaderLine(line, request); //header nuevo
				size_t colonPos = line.find(":");  //guarda el nombre si continua (multi-linea)
				if (colonPos != std::string::npos)
					lastHeaderName = trim(line.substr(0, colonPos));
				
		}
		pos = lineEnd + 2; //para saltar \r\n
	}
	//Parse Cookie
	std::map <std::string, std::string>::iterator cookieIt = request.headers.find("Cookie");
	if(cookieIt != request.headers.end()) 
	{
		CookieParser cookieParser;
		request.cookies = cookieParser.parse(cookieIt->second);
		//std::cout << B_CYAN << "[HeaderParsing] Parsed " << RESET << request.cookies.size() << " cookies" << std::endl;
	}
}