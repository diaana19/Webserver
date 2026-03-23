/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGIEnvironment.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vali <vali@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/15 17:05:17 by dianarituay       #+#    #+#             */
/*   Updated: 2026/03/08 19:03:46 by vali             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGIENVIRONMENT_HPP
#define CGIENVIRONMENT_HPP

#include "HttpRequest.hpp"
#include "Config.hpp"
#include "Colors.hpp"
#include <string>
#include <map>
// #include <chrono>

struct CGIProcess {
    pid_t       pid;
    int         outputPipeFd;   // fd to read CGI stdout from
    int         clientFd;    // fd to write request body to
    std::string buffer;       // accumulated output
    bool        readyToRead;
    time_t 		startTime; // time is only measure in SECOND from the first second
	CGIProcess(): outputPipeFd(-1), clientFd(-1), readyToRead(-1){};
};

class CGIEnvironment
{
	private:
		std::string intToString(int n);
		void parseScriptAndPath(const std::string& fullPath, const std::vector<std::string>& cgiExtensions, std::string& scriptName, std::string& pathInfo);
	public:
		CGIEnvironment();
		~CGIEnvironment();
		//construye array de variables de entorno, httprequest parseado de persona C y location de cgi_path, port
		char ** build(const HttpRequest& request, const LocationConfig& location, const ServerConfig& server);
};

#endif

/* ==== CGI ==== Common Gateway Interface
es un metodo q permite al servidor web ejecutar scripts(programas externos)
Flujo: Cliente: "Quiero un café personalizado"
		  ↓
Recepcionista: "No sé hacer café, llamo al chef"
		  ↓
Recepcionista llama al CHEF (script CGI)
		  ↓
Chef hace el café según especificaciones
		  ↓
Recepcionista entrega café al cliente
Servidor web solo sirve archivos estaticos (HTML, imagenes)
La solucion: contenido dinamico:
=> Cliente: dame info del usuario ID 42
=> Servidor: Ejecuto script que busca en base de datos
=> Script: Genera HTML personalizado
=> Servidor: Aquí está (diferente cada vez)
==== VARIABLES DE ENTORNO CGI =====
script -> empleado nuevo
variales -> manual de instrucciones
Variable | Descripción | Ejemplo |
|----------|-------------|---------|
| `REQUEST_METHOD` | Método HTTP | `"GET"`, `"POST"` |
| `QUERY_STRING` | Parámetros de URL | `"id=42&name=Diana"` |
| `CONTENT_TYPE` | Tipo de body | `"application/json"` |
| `CONTENT_LENGTH` | Tamaño del body | `"123"` |
| `PATH_INFO` | Path del script | `"/script.php"` |
| `SERVER_NAME` | Nombre del servidor | `"localhost"` |
| `SERVER_PORT` | Puerto | `"8080"` |
| `SCRIPT_NAME` | Nombre del script | `"/cgi-bin/script.php"` |
| `REMOTE_ADDR` | IP del cliente | `"127.0.0.1"` |
*/