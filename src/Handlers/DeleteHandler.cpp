/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DeleteHandler.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dianarituay <dianarituay@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/26 10:43:18 by dianarituay       #+#    #+#             */
/*   Updated: 2026/03/01 19:24:48 by dianarituay      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "DeleteHandler.hpp"
#include "PostHandler.hpp"

DeleteHandler::DeleteHandler(){};
DeleteHandler::~DeleteHandler(){};


bool DeleteHandler::verifyMethod(const std::string &method, const LocationConfig &location) {
	size_t i = 0;
	while (i < location.allow_methods.size()) {
		if (location.allow_methods[i] == method)
			 return (true);
		i++;
	}
	return (false);
}
bool DeleteHandler::doIsItExist(const std::string &pathfile) {
	struct stat buffer;
	if (stat(pathfile.c_str(), &buffer) == 0)
		return (true);
	return (false);
};
bool DeleteHandler::isRemoved(const std::string &pathfile) {
	int res = remove(pathfile.c_str());
	if (res == 0)
		return (true);
	return (false);
};

bool DeleteHandler::canDelete(const std::string &pathfile) {
	if (access(pathfile.c_str(), 2) == 0)
		return (true);
	return (false);
};

// taken trom GET 
std::string DeleteHandler::combinePath(const std::string &root, const std::string &uri)
{
    std::string path = root;
    if(!path.empty() && path[path.length() - 1] != '/')
        path += "/";

    std::string cleanUri = uri;
    if(!cleanUri.empty() && cleanUri[0] == '/')
        cleanUri = cleanUri.substr(1);
    return path + cleanUri;
}

HttpResponse DeleteHandler::handle(const HttpRequest &httpReq, const LocationConfig &location, const ServerConfig& server) {
	HttpResponse httpResponse;
	if (location.cgi_On)
	{
		PostHandler postHandler;
		return postHandler.handleCGI(httpReq, location, server);
	}
	std::string pathfile;
	pathfile = combinePath(location.rootLocation, httpReq.path);
	if (!(verifyMethod(httpReq.method, location))) {
		httpResponse.statusCode = 405; // "Method Not Allowed"
	}
	else if (pathfile.find("..") != std::string::npos) {
		httpResponse.statusCode = 403; // forbiden
	}
	else if (!(doIsItExist(pathfile))) {
		httpResponse.statusCode = 404; // not found
	}
	else if (!(canDelete(pathfile))) {
		httpResponse.statusCode = 403; // forbiden
	}
	else if (!(isRemoved(pathfile))) {
		httpResponse.statusCode = 403; // forbiden
	}
	else
		httpResponse.statusCode = 204; // No content // OK 
	return (httpResponse);
};
