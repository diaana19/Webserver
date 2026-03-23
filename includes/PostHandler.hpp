/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PostHandler.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vali <vali@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/13 15:18:12 by dianarituay       #+#    #+#             */
/*   Updated: 2026/03/08 18:23:47 by vali             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef POSTHANDLER_HPP
#define POSTHANDLER_HPP

#include "Config.hpp"
#include "HttpRequest.hpp"
#include "HttpResponse.hpp"
#include "Colors.hpp"
#include <string>
#include <cstdlib>

class PostHandler
{
	private:
			HttpResponse handleFormData(const HttpRequest& request, const LocationConfig& location);
			HttpResponse handleFileUpload(const HttpRequest &request, const LocationConfig &location);
			std::string intToString(int n);
	public:
			HttpResponse parseOutput(const std::string& output);
			PostHandler();
			~PostHandler();
			HttpResponse handle(const HttpRequest &request, const LocationConfig &location, const ServerConfig& server);
			HttpResponse handleCGI(const HttpRequest &request, const LocationConfig &location, const ServerConfig& server);

};	

#endif

/* El servidor puede:
GET => servir archivos, cliente PIDE "dame el file index.html"
POST => recibir datos y uploads, cliente ENVIA algo al servidor, "guarda estos datos", "sube este archivo"
DELETE => eliminar archivos
PUT => "actualiza esto", modifica el file
====== POST ======
1. formulario simple, application/x-www-form-urlencoded
-> usos: forms de contacto, login, busquedas
2. multipart/form-data (con files) tiene boundary
-> usos: upload de fotos, de documentos, formularios adjuntos
3. application/json (APIs modernas)
-> usos: APIs REST, apps web modernas, comunicacion entre servicios
*/