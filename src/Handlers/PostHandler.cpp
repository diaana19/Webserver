#include "PostHandler.hpp"
#include "CGIEnvironment.hpp"
#include "CGIExecutor.hpp"
#include "CGIPipes.hpp"
#include <sys/wait.h>
#include <unistd.h>
#include <sys/wait.h> 
#include <signal.h> 
#include <fstream> 
#include <sstream> 
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
	ss << n; 
	return (ss.str()); 
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
	if (request.body.empty() && request.allFiles.empty()) 
	{
		HttpResponse response;
		response.statusCode = 400;
		response.headers["Content-Type"] = "text/html";
		response.body = "<!DOCTYPE html><html><body>"
							"<h1>Error 400 - Bad Request</h1>"
							"<p>No data received</p>"
							"</body></html>";
		return (response); 
	}
	if (!request.allFiles.empty() && !request.allFiles[0].filename.empty()) 
		return (handleFileUpload(request, location));
	return (handleFormData(request, location));
}


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
	
	std::ofstream file("www/data/form_submissions.txt", std::ios::app); 
	if (file.is_open())
	{
		file << request.body << std::endl; 
		file.close();
		response.statusCode = 302;
		response.headers["Location"] = "/success_form.html";
	}
	else 
	{
		response.statusCode = 500; 
		response.headers["Content-Type"] = "text/html";
		response.body = "<!DOCTYPE html><html><body>"
							 "<h1>Error 500 - Internal Server Error</h1>"
							 "<p>Failed to save form data</p>"
							 "</body></html>";
	}
	return (response);
}


HttpResponse PostHandler::handleFileUpload(const HttpRequest &request, const LocationConfig &location)
{
	HttpResponse response;
	if (location.uploadPath.empty()) 
	{
		response.statusCode = 500; 
		response.headers["Content-Type"] = "text/html";
		response.body = "<!DOCTYPE html><html><body>"
							 "<h1>Error 500 - Internal Server Error</h1>"
							 "<p>Upload path is not configured for this location</p>"
							 "</body></html>";
		return (response);
	}
	
	for (size_t i = 0; i < request.allFiles.size(); i++)
	{
		const File& file = request.allFiles[i];
		if (file.filename.empty())
			continue ;
		
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
		
		std::ofstream outFile(fullPath.c_str(), std::ios::binary | std::ios::trunc);
		if (!outFile.is_open())
		{
			response.statusCode = 500; 
			response.headers["Content-Type"] = "text/html";
			response.body = "<!DOCTYPE html><html><body>"
								 "<h1>Error 500 - Internal Server Error</h1>"
								 "<p>Failed to save file: " + file.filename + "</p>"
								 "</body></html>";
			return (response);
		}
		
		outFile.write(file.file_buffer.c_str(), file.file_buffer.size());
		outFile.close();
	}
	
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
	
	response.statusCode = 302;
	response.headers["Location"] = "/success_upload.html";
	return (response);
}

HttpResponse PostHandler::parseOutput(const std::string &output)
{
	HttpResponse response;
	
	size_t headerEnd = output.find("\r\n\r\n");

	
	if (headerEnd == std::string::npos)
	{
		headerEnd = output.find("\n\n");
	}

	
	if (headerEnd == std::string::npos)
	{
		std::cout << RED << "[CGI] No headers found, treating all as body" << RESET << std::endl;

		response.statusCode = 200;
		response.headers["Content-Type"] = "text/html";
		response.body = output;
		return response;
	}
	
	std::string headersStr = output.substr(0, headerEnd);

	
	if (output[headerEnd] == '\r')
	{
		
		response.body = output.substr(headerEnd + 4);
	}
	else
	{
		
		response.body = output.substr(headerEnd + 2);
	}

	std::cout << B_CYAN << "[CGI] Headers size: " << RESET << headersStr.size() << " bytes" << std::endl;
	std::cout << B_CYAN << "[CGI] Body size: " << RESET << response.body.size() << " bytes" << std::endl;
	
	std::istringstream stream(headersStr);
	std::string line;

	
	while (std::getline(stream, line))
	{
		
		if (line.empty() || line == "\r")
			continue;
		
		
		size_t colon = line.find(':');

		if (colon != std::string::npos)
		{
			std::string key = line.substr(0, colon);
			std::string value = line.substr(colon + 1);
			if (!value.empty() && value[0] == ' ')
				value = value.substr(1);
			if (!value.empty() && value[value.size() - 1] == '\r')
				value = value.substr(0, value.size() - 1);

			
			if (key == "Set-Cookie")
				response.setCookieHeaders.push_back("Set-Cookie: " + value + "\r\n");
			else
				response.headers[key] = value;
			std::cout << B_CYAN << "[CGI] Header: " << RESET << key << " = " << value << std::endl;
        }	
	}
	
	response.statusCode = 200;

	
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
    std::cout << B_CYAN << "[CGI] Interpreter: " << RESET << interpreter << std::endl;  
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
        cgiPipe.closePipes(pipeIn, pipeOut); 
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
    close(pipeIn[1]);  
    

    std::cout << B_GREEN << "[OK]" <<  " [CGI] Input pipe closed (EOF sent to CGI)" << RESET << std::endl;

    HttpResponse response;
    response.cgiProcess.pid = pid;
    response.cgiProcess.outputPipeFd = pipeOut[0];
    response.cgiProcess.readyToRead = false;
    response.cgiProcess.startTime = time(NULL);
    return (response);
}