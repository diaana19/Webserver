/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HeadersBuilder.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dianarituay <dianarituay@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/01 17:34:58 by dianarituay       #+#    #+#             */
/*   Updated: 2026/02/02 12:56:03 by dianarituay      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HEADERSBUILDER_HPP
#define HEADERSBUILDER_HPP

#include "HttpResponse.hpp"
#include <string>
#include <map>

class HeadersBuilder
{
	private:
		std::string getDateHeader();
		std::string getServerHeader();
		std::string intToString(int n);
	public:
		HeadersBuilder();
		~HeadersBuilder();
		std::string build(const HttpResponse& response);
};

#endif


/*
		**FLUJO COMPLETO:**
1. Se abre Firefox -> http://localhost:8080/index.html
2. Firefox crea un request y dice necesito pedirle al servidor index.html entonces firefox construye un sms:
					GET /index.html HTTP/1.1
					Host: localhost:8080
					User-Agent: Mozilla/5.0
					Accept: text/html
3. Firefox se conecta a nuestro servidor y envia el request por el socket, recibimos el request en bytes
4. Parseamos el request y sabemos q Firefox quiere /index.html y quiere la rpta en html
5. Buscamos el archivo y leemos el disco duro(body)
6. Construimos la respuesta:
						HTTP/1.1 200 OK -> status line
						Content-Type: text/html -> headers
						Content-Length: 31
						Server: webserv/1.0
						*linea vacia*
						<html><body>Hola!</body></html> -> body
7. Enviamos la rpta a Firefox y este la parsea
8. Firefox renderiza la pagina (la muestra) => Hola! y la pagina aparece en el navegador
*/