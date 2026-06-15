#ifndef HEADERSPARSING_HPP
# define HEADERSPARSING_HPP

#include "HttpRequest.hpp"

class HeadersParsing
{
  private:
	size_t findHeadersEnd(const std::string& rawRequest);
	void parseHeaderLine(const std::string& line, HttpRequest& request);
	bool isMultiLineHeader(const std::string& line);
	std::string trim(const std::string& str);
  
  public:
	HeadersParsing();
	~HeadersParsing();

	void parse(const std::string& rawRequest, HttpRequest& request);
};

#endif