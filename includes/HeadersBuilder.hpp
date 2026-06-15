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