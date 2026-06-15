#include "CGIEnvironment.hpp"
#include <sstream> 
#include <cstdlib> 

CGIEnvironment::CGIEnvironment()
{

}

CGIEnvironment::~CGIEnvironment()
{
	
}

std::string CGIEnvironment::intToString(int n)
{
	std::stringstream ss;
	ss << n;				 
	return (ss.str()); 
}

void CGIEnvironment::parseScriptAndPath(const std::string& fullPath, const std::vector<std::string>& cgiExtensions, std::string& scriptName, std::string& pathInfo)
{
	for (size_t ext = 0; ext < cgiExtensions.size(); ext++)
	{
		std::string extension = cgiExtensions[ext]; 
		size_t pos = fullPath.find(extension);
		if (pos != std::string::npos)
		{
			size_t endScript = pos + extension.size();
			scriptName = fullPath.substr(0, endScript);
			if (endScript < fullPath.size())
				pathInfo = fullPath.substr(endScript);
			else
				pathInfo = "";
			return ;
		}
	}
	scriptName = fullPath;
	pathInfo = "";
}


char** CGIEnvironment::build(const HttpRequest& request, const LocationConfig& location, const ServerConfig& server)
{
	char** envp = new char*[30];
	int i = 0;

	
	envp[i++] = strdup("GATEWAY_INTERFACE=CGI/1.1");
	
	
	envp[i++] = strdup("SERVER_SOFTWARE=webserver/1.0");
	
	
	std::string method = "REQUEST_METHOD=" + request.method;
	envp[i++] = strdup(method.c_str()); 
	
	
	std::string queryString;
    size_t queryPos = request.path.find('?');
    if (queryPos != std::string::npos) 
        queryString = request.path.substr(queryPos + 1);
    else if (!request.queryString.empty()) 
        queryString = request.queryString;
    else 
        queryString = "";
    std::string query = "QUERY_STRING=" + queryString;
    envp[i++] = strdup(query.c_str());
    
    std::cout << B_CYAN << "[CGI] QUERY_STRING=" << RESET << queryString << std::endl;
    
	
	std::string serverName;
	std::map<std::string, std::string>::const_iterator hostIt;
	hostIt = request.headers.find("Host");
	if (hostIt != request.headers.end())
	{
		std::string host = hostIt->second; 
		size_t colon = host.find(':'); 
		if (colon != std::string::npos)
			host = host.substr(0, colon); 
		serverName = "SERVER_NAME=" + host;
	}
	else
		serverName = "SERVER_NAME=" + server.host;
	envp[i++] = strdup(serverName.c_str());

	
	std::string port = "SERVER_PORT=" + intToString(server.port);
	envp[i++] = strdup(port.c_str());

	
	
	  std::string pathWithoutQuery = request.path;
    queryPos = pathWithoutQuery.find('?');
    if (queryPos != std::string::npos) 
        pathWithoutQuery = pathWithoutQuery.substr(0, queryPos);
    std::string scriptName;
    std::string pathInfo;
    parseScriptAndPath(pathWithoutQuery, location.cgi_extension, scriptName, pathInfo);
    std::string scriptNameVar = "SCRIPT_NAME=" + scriptName;
    envp[i++] = strdup(scriptNameVar.c_str());
    std::cout << B_CYAN << "[CGI] SCRIPT_NAME=" << RESET << scriptName << std::endl;
    if (!pathInfo.empty()) 
	{
        std::string pathInfoVar = "PATH_INFO=" + pathInfo;
        envp[i++] = strdup(pathInfoVar.c_str());
        std::string pathTranslated = "PATH_TRANSLATED=" + server.root + pathInfo;
        envp[i++] = strdup(pathTranslated.c_str());
        std::cout << "[CGI] PATH_INFO=" << pathInfo << std::endl;
    }

	
	std::map<std::string, std::string>::const_iterator ctIt;
	ctIt = request.headers.find("Content-Type");
	if (ctIt != request.headers.end())
	{
		std::string ct = "CONTENT_TYPE=" + ctIt->second;
		envp[i++] = strdup(ct.c_str());
	}
	
	
	if (!request.body.empty())
	{
		std::string cl = "CONTENT_LENGTH=" + intToString(request.body.size());
		envp[i++] = strdup(cl.c_str());
	}

	
	std::string protocol = "SERVER_PROTOCOL=" + request.httpVersion;
	envp[i++] = strdup(protocol.c_str());
	
	
	std::string remoteAddr = "REMOTE_ADDR=" + request.clientIP;
	envp[i++] = strdup(remoteAddr.c_str());
	
	
	std::map<std::string, std::string>::const_iterator uaIt;
   uaIt = request.headers.find("User-Agent");
   if (uaIt != request.headers.end()) 
	{
   	std::string ua = "HTTP_USER_AGENT=" + uaIt->second;
      envp[i++] = strdup(ua.c_str());
	}

	
	std::map<std::string, std::string>::const_iterator acceptIt;
   acceptIt = request.headers.find("Accept");
   if (acceptIt != request.headers.end()) 
	{
		std::string accept = "HTTP_ACCEPT=" + acceptIt->second;
		envp[i++] = strdup(accept.c_str());
	}

	
	std::map<std::string, std::string>::const_iterator cookieIt;
   cookieIt = request.headers.find("Cookie");
   if (cookieIt != request.headers.end()) 
	{
		std::string cookie = "HTTP_COOKIE=" + cookieIt->second;
		envp[i++] = strdup(cookie.c_str());
	}

	
	std::map<std::string, std::string>::const_iterator refIt;
   refIt = request.headers.find("Referer");
   if (refIt != request.headers.end()) 
	{	std::string referer = "HTTP_REFERER=" + refIt->second;
      envp[i++] = strdup(referer.c_str());
	}

	
	std::map<std::string, std::string>::const_iterator langIt;
   langIt = request.headers.find("Accept-Language");
   if (langIt != request.headers.end()) 
	{
      std::string lang = "HTTP_ACCEPT_LANGUAGE=" + langIt->second;
      envp[i++] = strdup(lang.c_str());
	}
	
	envp[i] = NULL;
	return (envp);
}