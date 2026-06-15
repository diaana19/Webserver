#include "RequestLine.hpp"
#include <sstream>
#include <iostream>
#include <cctype>

RequestLine:: RequestLine()
{}

RequestLine:: ~RequestLine()
{}

std::string RequestLine::trim(const std::string &str)
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


void RequestLine::parsePath(const std::string &fullPath, std::string &path, std::string &query)
{
	size_t questionMark = fullPath.find('?');

	
	if(questionMark != std::string::npos)
	{
		path = fullPath.substr(0, questionMark); 
		query = fullPath.substr(questionMark + 1); 
	}
	else
	{
		path = fullPath;
		query = "";
	}
}

bool RequestLine::parse(const std::string &line, HttpRequest &req)
{
	std::string clean_line = trim(line);

	if(clean_line.empty()) {
		std::cerr << "Error: request line is empty" << std::endl;
		return false;
	}

	std::istringstream div_line(clean_line);
	std::string method, fullpath, version;

	
	if(!(div_line >> method >> fullpath >> version)) {
		std::cerr << "Error: invalid request line format" << std::endl;
		return false;
	}

	if(method != "GET" && method != "POST" && method != "DELETE" && method != "HEAD" && method != "PUT" && method != "OPTIONS") {
		std::cerr << "Error: invalid http method" << std::endl;
		return false;
	}

	
	if(version != "HTTP/1.0" && version != "HTTP/1.1") {
		std::cerr << "Error: invalid http version: " << version << std::endl;
		return false;
	}


	
	if(fullpath.empty() || fullpath[0] != '/') {
		std::cerr << "Error: invalid path must start with '/' " << std::endl;
		return false;
	}

	
	if(fullpath.find("..") != std::string::npos) {
		std::cerr << "[SECURITY] Path transversal detected: " << fullpath << std::endl;
		return false;
	}

	
	std::string decoded = urlDecode(fullpath);


	if(decoded.find("..") != std::string::npos){
		std::cerr << " [SECURITY] Encoded path transversal detected: " << fullpath << "(decoded: " << decoded << ")" << std::endl;
		return false;
	}

	
	std::string normalized = normalizePath(decoded);

	
	std::string path, query;
	parsePath(normalized, path, query);

	
	req.method = method;
	req.path = path;
	req.queryString = query;
	req.httpVersion = version;

	return true;
}

std::string RequestLine::urlDecode(const std::string& str)
{
    std::string result;
    
    for(size_t i = 0; i < str.length(); i++)
    {
        if(str[i] == '%' && i + 2 < str.length())
        {
            std::string hex = str.substr(i + 1, 2);
            
            
            char ch = 0;
            for(int j = 0; j < 2; j++)
            {
                char c = hex[j];
                ch *= 16;
                
                if(c >= '0' && c <= '9')
                    ch += c - '0';
                else if(c >= 'a' && c <= 'f')
                    ch += c - 'a' + 10;
                else if(c >= 'A' && c <= 'F')
                    ch += c - 'A' + 10;
            }
            
            result += ch;
            i += 2; 
        }
        else if(str[i] == '+')
        {
            result += ' '; 
        }
        else
        {
            result += str[i];
        }
    }
    
    return result;
}


std::string RequestLine::normalizePath(const std::string &path)
{
	std::vector <std::string> parts;
	std::istringstream iss(path);
	std::string part;

	
	while(std::getline(iss, part, '/')) {
		if(part.empty() || part == ".")
			continue;
		if(part == "..") {
			
			if(!parts.empty())
				parts.pop_back();
			
		}
		else
			parts.push_back(part);
	}

	
	std::string result = "/";
	for(size_t i = 0; i<parts.size(); i++) {
		if(i>0)
			result += "/";
		result += parts[i];
	}

	return result;
}