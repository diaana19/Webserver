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