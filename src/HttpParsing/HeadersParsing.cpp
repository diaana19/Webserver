#include "HeadersParsing.hpp"
#include "CookieParser.hpp"
#include <stdexcept>
#include <string>

HeadersParsing::HeadersParsing()
{
}

HeadersParsing::~HeadersParsing()
{
}
std::string HeadersParsing::trim(const std::string &str)
{
	size_t	start;
	size_t	end;

	if (str.empty())
		return (str);
	
	start = 0;
	while (start < str.size() && (str[start] == ' ' || str[start] == '\t'))
		start++;
	
	end = str.size();
	while (end > start && (str[end - 1] == ' ' || str[end - 1] == '\t'))
		end--;
	return (str.substr(start, end - start));
}


size_t HeadersParsing::findHeadersEnd(const std::string &rawRequest)
{
	size_t pos = rawRequest.find("\r\n\r\n"); 
	if (pos == std::string::npos)
		throw std::runtime_error("Error: hearders end not found");
	return (pos);
}


void HeadersParsing::parseHeaderLine(const std::string &line, HttpRequest &request)
{
	size_t	colonPos;

	if (line.empty())
		return ;
	colonPos = line.find(":");
	if (colonPos == std::string::npos)
		return ; 
	std::string name = trim(line.substr(0, colonPos));
	std::string value;
	if (colonPos + 1 < line.size()) 
		value = trim(line.substr(colonPos + 1));
	else
		value = ""; 
	request.headers[name] = value; 
}


bool HeadersParsing::isMultiLineHeader(const std::string &line)
{
	if (line.empty())
		return (false);
	if (line[0] == ' ' || line[0] == '\t')
		return (true);
	return (false);
}

void HeadersParsing::parse(const std::string &rawRequest, HttpRequest &request)
{
	size_t firstLineEnd = rawRequest.find("\r\n");
	if (firstLineEnd == std::string::npos)
		return ;
	size_t headersStart = firstLineEnd + 2; 
	size_t headersEnd = findHeadersEnd(rawRequest);
	std::string headersSection = rawRequest.substr(headersStart, headersEnd - headersStart); 
	size_t pos = 0; 
	std::string lastHeaderName = ""; 
	while (pos < headersSection.size())
	{
		size_t lineEnd = headersSection.find("\r\n", pos);
		if (lineEnd == std::string::npos)
			lineEnd = headersSection.size();
		std::string line = headersSection.substr(pos, lineEnd - pos); 
		
		if (line.empty())
			break ;	
		if (isMultiLineHeader(line))
		{
			if (!lastHeaderName.empty())
				request.headers[lastHeaderName] += " " + trim(line); 
		}
		else
		{
				parseHeaderLine(line, request); 
				size_t colonPos = line.find(":");  
				if (colonPos != std::string::npos)
					lastHeaderName = trim(line.substr(0, colonPos));
				
		}
		pos = lineEnd + 2; 
	}
	
	std::map <std::string, std::string>::iterator cookieIt = request.headers.find("Cookie");
	if(cookieIt != request.headers.end()) 
	{
		CookieParser cookieParser;
		request.cookies = cookieParser.parse(cookieIt->second);
		
	}
}