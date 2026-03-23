/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PostHandler.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vali <vali@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/13 15:17:46 by dianarituay       #+#    #+#             */
/*   Updated: 2026/03/19 12:52:37 by vali             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "PostHandler.hpp"
#include "CGIEnvironment.hpp"
#include "CGIExecutor.hpp"
#include "CGIPipes.hpp"
#include <sys/wait.h>
#include <unistd.h>
#include <sys/wait.h> //waitpid()
#include <signal.h> //kill()
#include <fstream> //leer escribir files
#include <sstream> //intToString()
#include <iostream>
#include <map>

PostHandler::PostHandler()
{

}

PostHandler::~PostHandler()
{

}
std::string PostHandler::intToString(int n)
{
	std::stringstream ss;
	ss << n; //inserta n en el stream
	return (ss.str()); // asi obtenemos el str
}


HttpResponse PostHandler::handle(const HttpRequest &request, const LocationConfig& location, const ServerConfig& server)
{	
	if (!location.allow_methods.empty())
    {
        bool allowed = false;
        for (size_t i = 0; i < location.allow_methods.size(); i++)
        {
            if (location.allow_methods[i] == request.method)
            {
                allowed = true;
                break;
            }
        }
        if (!allowed)
        {
            HttpResponse response;
            response.statusCode = 405;
            return (response);
        }
    }
	if (location.cgi_On)
		return (handleCGI(request, location, server));
	if (request.body.empty() && request.allFiles.empty()) // si POST vacio o npos
	{//crea rpta de error
		HttpResponse response;
		response.statusCode = 400;
		response.headers["Content-Type"] = "text/html";
		response.body = "<!DOCTYPE html><html><body>"
							"<h1>Error 400 - Bad Request</h1>"
							"<p>No data received</p>"
							"</body></html>";
		return (response); 
	}
	if (!request.allFiles.empty() && !request.allFiles[0].filename.empty()) //form files
		return (handleFileUpload(request, location));
	return (handleFormData(request, location));
}

//formularios simples, sin archivos y los guarda en files de texto
HttpResponse PostHandler::handleFormData(const HttpRequest &request, const LocationConfig &location)
{
	(void)location;
	HttpResponse response;
	if (request.body.empty())
	{
		response.statusCode = 400;
		response.headers["Content-Type"] = "text/html";
		response.body = "<!DOCTYPE html><html><body>"
							 "<h1>Error 400 - Bad Request</h1>"
							 "<p>No data received</p>"
							 "</body></html>";
		return (response);
	}
	//guarda los datos en el file
	std::ofstream file("www/data/form_submissions.txt", std::ios::app); //abre files para escritura
	if (file.is_open())
	{
		file << request.body << std::endl; //agrega append(app)
		file.close();
		response.statusCode = 302;
		response.headers["Location"] = "/success_form.html";
	}
	else //no se pudo abrir file
	{
		response.statusCode = 500; //internal server error
		response.headers["Content-Type"] = "text/html";
		response.body = "<!DOCTYPE html><html><body>"
							 "<h1>Error 500 - Internal Server Error</h1>"
							 "<p>Failed to save form data</p>"
							 "</body></html>";
	}
	return (response);
}

//forms con files y los guarda en disco
HttpResponse PostHandler::handleFileUpload(const HttpRequest &request, const LocationConfig &location)
{
	HttpResponse response;
	if (location.uploadPath.empty()) //si upload esta configurado
	{
		response.statusCode = 500; // internal server error
		response.headers["Content-Type"] = "text/html";
		response.body = "<!DOCTYPE html><html><body>"
							 "<h1>Error 500 - Internal Server Error</h1>"
							 "<p>Upload path is not configured for this location</p>"
							 "</body></html>";
		return (response);
	}
	//guarda cada files
	for (size_t i = 0; i < request.allFiles.size(); i++)
	{
		const File& file = request.allFiles[i];
		if (file.filename.empty())
			continue ;
		// Sanitize filename: keep only basename, reject path traversal
		std::string safeFilename = file.filename;
		size_t lastSlash = safeFilename.find_last_of("/\\");
		if (lastSlash != std::string::npos)
			safeFilename = safeFilename.substr(lastSlash + 1);
		if (safeFilename.empty() || safeFilename.find("..") != std::string::npos)
		{
			response.statusCode = 400;
			response.headers["Content-Type"] = "text/html";
			response.body = "<!DOCTYPE html><html><body>"
								 "<h1>Error 400 - Bad Request</h1>"
								 "<p>Invalid filename</p>"
								 "</body></html>";
			return (response);
		}
		std::string fullPath = location.uploadPath + safeFilename;
		//binary -> imagenes, pdfs trunc-> sobreescribe
		std::ofstream outFile(fullPath.c_str(), std::ios::binary | std::ios::trunc);
		if (!outFile.is_open())
		{
			response.statusCode = 500; // internal server error
			response.headers["Content-Type"] = "text/html";
			response.body = "<!DOCTYPE html><html><body>"
								 "<h1>Error 500 - Internal Server Error</h1>"
								 "<p>Failed to save file: " + file.filename + "</p>"
								 "</body></html>";
			return (response);
		}
		//escribir datos binarios
		outFile.write(file.file_buffer.c_str(), file.file_buffer.size());
		outFile.close();
	}
	//crea lista HTML
	std::string filesList;
	for (size_t i = 0; i < request.allFiles.size(); i++)
	{
		if (!request.allFiles[i].filename.empty())
		{
			std::string name = request.allFiles[i].filename;
			size_t sl = name.find_last_of("/\\");
			if (sl != std::string::npos)
				name = name.substr(sl + 1);
			filesList += "<li>" + name + " (" + intToString(request.allFiles[i].file_buffer.size()) + " bytes)</li>";
		}
	}
	//crear rpta
	response.statusCode = 302;
	response.headers["Location"] = "/success_upload.html";
	return (response);
}

HttpResponse PostHandler::parseOutput(const std::string &output)
{
	HttpResponse response;
	//encontrar separacion headers/body
	size_t headerEnd = output.find("\r\n\r\n");

	// Si no encuentra \r\n\r\n, buscar \n\n
	if (headerEnd == std::string::npos)
	{
		headerEnd = output.find("\n\n");
	}

	// Si NO hay separación, asumir que todo es body
	if (headerEnd == std::string::npos)
	{
		std::cout << RED << "[CGI] No headers found, treating all as body" << RESET << std::endl;

		response.statusCode = 200;
		response.headers["Content-Type"] = "text/html";
		response.body = output;
		return response;
	}
	//separar headers y body
	std::string headersStr = output.substr(0, headerEnd);

	// Body: desde después de la línea vacía hasta el final
	if (output[headerEnd] == '\r')
	{
		// Formato \r\n\r\n - saltar 4 caracteres
		response.body = output.substr(headerEnd + 4);
	}
	else
	{
		// Formato \n\n - saltar 2 caracteres
		response.body = output.substr(headerEnd + 2);
	}

	std::cout << B_CYAN << "[CGI] Headers size: " << RESET << headersStr.size() << " bytes" << std::endl;
	std::cout << B_CYAN << "[CGI] Body size: " << RESET << response.body.size() << " bytes" << std::endl;
	// Crear stream para leer línea por línea
	std::istringstream stream(headersStr);
	std::string line;

	// Leer cada línea del header
	while (std::getline(stream, line))
	{
		// Ignorar líneas vacías
		if (line.empty() || line == "\r")
			continue;
		// formato header: Content-Type: text/html
		// Buscar el ':'
		size_t colon = line.find(':');

		if (colon != std::string::npos)
		{
			std::string key = line.substr(0, colon);
			std::string value = line.substr(colon + 1);
			if (!value.empty() && value[0] == ' ')
				value = value.substr(1);
			if (!value.empty() && value[value.size() - 1] == '\r')
				value = value.substr(0, value.size() - 1);

			// ← Set-Cookie va al vector, no al map
			if (key == "Set-Cookie")
				response.setCookieHeaders.push_back("Set-Cookie: " + value + "\r\n");
			else
				response.headers[key] = value;
			std::cout << B_CYAN << "[CGI] Header: " << RESET << key << " = " << value << std::endl;
        }	
	}
	// Status code por defecto (si el CGI no lo especifica)
	response.statusCode = 200;

	// Si el CGI no especificó Content-Type, poner default
	if (response.headers.find("Content-Type") == response.headers.end())
	{ 
		response.headers["Content-Type"] = "text/html";
		std::cout << RED << "[CGI] No Content-Type header, using text/html" << RESET << std::endl;
	}
	std::cout << B_GREEN << "[OK]" << " [CGI] Response parsed successfully" << RESET << std::endl;
	return (response);
}

void freeEnvp(char **envp)
{
    if (!envp)
        return;
    for (int i = 0; envp[i] != NULL; i++)
        free(envp[i]);
    delete[] envp; 
}


HttpResponse PostHandler::handleCGI(const HttpRequest &request, const LocationConfig &location, const ServerConfig &server)
{
    std::string scriptPath = location.cgi_path;
    
    if (scriptPath.empty())
    {
        scriptPath = location.rootLocation;
        if (!scriptPath.empty() && scriptPath[scriptPath.length() - 1] != '/')
            scriptPath += "/";
        std::string cleanPath = request.path;
        if (!cleanPath.empty() && cleanPath[0] == '/')
            cleanPath = cleanPath.substr(1);
        scriptPath += cleanPath;
    }
    else
    {
        std::string pathWithoutQuery = request.path;
        size_t queryPos = pathWithoutQuery.find('?');
        if (queryPos != std::string::npos)
            pathWithoutQuery = pathWithoutQuery.substr(0, queryPos);
        std::string filename = pathWithoutQuery;
        size_t lastSlash = filename.find_last_of('/');
        if (lastSlash != std::string::npos)
            filename = filename.substr(lastSlash + 1);
        if (!scriptPath.empty() && scriptPath[scriptPath.length() - 1] != '/')
            scriptPath += "/";
        scriptPath += filename;
    }

    std::string interpreter = location.cgi_interpreter;
    if (scriptPath.find(".php") != std::string::npos)
        interpreter = "/usr/bin/php";
    else if (scriptPath.find(".py") != std::string::npos)
        interpreter = "/usr/bin/python3";
    else if (scriptPath.find(".sh") != std::string::npos)
        interpreter = "/bin/bash";

    std::cout << B_CYAN << "[CGI] Script path: " << RESET << scriptPath << std::endl;
    std::cout << B_CYAN << "[CGI] Interpreter: " << RESET << interpreter << std::endl;  // ← fix log
    std::cout << B_CYAN << "[CGI] Request method: " << RESET << request.method << std::endl;

    CGIEnvironment cgiEnv;

    CGIPipes pipes;
    int pipeIn[2]  = {-1, -1};
	int pipeOut[2] = {-1, -1};

    try
    {
        pipes.setupPipes(pipeIn, pipeOut);
		std::cout << B_GREEN << "[OK]" << " [CGI] Pipes created successfully" << RESET << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cerr << RED << "[CGI] Failed to create pipes: " << RESET << e.what() << std::endl;
        HttpResponse response;
        response.statusCode = 500;
        response.headers["Content-Type"] = "text/html";
        response.body = "<!DOCTYPE html><html><body>"
                        "<h1>Error 500 - Internal Server Error</h1>"
                        "<p>Failed to create communication pipes</p>"
                        "</body></html>";
        return (response);
    }

    CGIExecutor executor;
    char **envp = cgiEnv.build(request, location, server);
    pid_t pid = executor.execute(request, scriptPath, interpreter, envp, pipeIn, pipeOut);
    freeEnvp(envp);

    if (pid == -1)
    {
        std::cerr << RED << "[CGI] Failed to execute script" << RESET << std::endl;
		CGIPipes cgiPipe;
        close(pipeIn[1]);
        close(pipeOut[0]);
        cgiPipe.closePipes(pipeIn, pipeOut); // close all pipe for now if failed to execute cgi 
        HttpResponse response;
        response.statusCode = 500;
        response.headers["Content-Type"] = "text/html";
        response.body = "<!DOCTYPE html><html><body>"
                        "<h1>Error 500 - Internal Server Error</h1>"
                        "<p>Failed to execute CGI script</p>"
                        "</body></html>";
        return (response);
    }

    std::cout << B_CYAN << "[CGI] Process started with PID: " << RESET << pid << std::endl;

    // ← FIX: escribir body ANTES de cerrar el pipe
    if (!request.body.empty())
    {
		fcntl(pipeIn[1], F_SETFL, O_NONBLOCK);
        ssize_t written = write(pipeIn[1], request.body.c_str(), request.body.size());
		if (written < 0)
        	std::cerr << RED << "[CGI] Failed to write body to pipe" << RESET << std::endl;
    	else if ((size_t)written < request.body.size())
        	std::cerr << "[CGI] Partial write: " << written << "/" << request.body.size() << " bytes" << std::endl;
        std::cout << B_CYAN << "[CGI] Body written to stdin: " << RESET << request.body.size() << " bytes" << std::endl;
    }
    close(pipeIn[1]);  // ahora sí, EOF al hijo
    //close(pipeOut[1]); // el padre no escribe en pipeOut (ya cerrado en execute() pero por seguridad)

    std::cout << B_GREEN << "[OK]" <<  " [CGI] Input pipe closed (EOF sent to CGI)" << RESET << std::endl;

    HttpResponse response;
    response.cgiProcess.pid = pid;
    response.cgiProcess.outputPipeFd = pipeOut[0];
    response.cgiProcess.readyToRead = false;
    response.cgiProcess.startTime = time(NULL);
    return (response);
}

/* ==== MULTIPART ====
para enviar varios datos en un solo POST, como name, email, foto separados por un "boundary" -> ------WebKit123
====== FLUJO ======
1. usuario llena el formulario -> 2. enviar
3. form HTML crea POST -> 4.firefox crea mensaje HTTP (status lines, headers, body) -> 5. firefox envia por socket(convierte a bytes y envia po red) -> 6. mi server recibe bytes -> 7. parsea(requestLine, headers, body) -> 8. posthandler guarda los datos -> 9. httpresponse crea respuesta con HTML -> 10. servidor envia rpta(statusline, headers, body) -> 11. firefox recibe respuesta -> 12. firefox muestra HTML
*/
