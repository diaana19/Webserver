/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestLine.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dianarituay <dianarituay@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/01 17:12:08 by dianarituay       #+#    #+#             */
/*   Updated: 2026/02/01 17:52:04 by dianarituay      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
	// encontrar primer carácter que NO sea espacio/tab
	start = 0;
	while (start < str.size() && (str[start] == ' ' || str[start] == '\t'))
		start++;
	// encontrar último carácter que NO sea espacio/tab
	end = str.size(); //xq no el length ? 
	while (end > start && (str[end - 1] == ' ' || str[end - 1] == '\t')) // xq no funciones isspace etc :D
		end--;
	return (str.substr(start, end - start));
}

//separation path of the query string
void RequestLine::parsePath(const std::string &fullPath, std::string &path, std::string &query)
{
	size_t questionMark = fullPath.find('?');

	//std::string::npos until the end of the string who is fullpath
	if(questionMark != std::string::npos)
	{
		path = fullPath.substr(0, questionMark); //el path desde inicio hasta ?
		query = fullPath.substr(questionMark + 1); //1 position after to dont take the ? 
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

	//parsing request
	if(!(div_line >> method >> fullpath >> version)) {
		std::cerr << "Error: invalid request line format" << std::endl;
		return false;
	}

	if(method != "GET" && method != "POST" && method != "DELETE" && method != "HEAD" && method != "PUT" && method != "OPTIONS") {
		std::cerr << "Error: invalid http method" << std::endl;
		return false;
	}

	//valid version NEW ADDDDD 
	if(version != "HTTP/1.0" && version != "HTTP/1.1") {
		std::cerr << "Error: invalid http version: " << version << std::endl;
		return false;
	}


	// NEW VALIDATION : Verify is start with /
	if(fullpath.empty() || fullpath[0] != '/') {
		std::cerr << "Error: invalid path must start with '/' " << std::endl;
		return false;
	}

	//NEW ADD detection of transversal path 
	if(fullpath.find("..") != std::string::npos) {
		std::cerr << "[SECURITY] Path transversal detected: " << fullpath << std::endl;
		return false;
	}

	//New validation : Decodification and verification %2e%2e
	std::string decoded = urlDecode(fullpath);


	if(decoded.find("..") != std::string::npos){
		std::cerr << " [SECURITY] Encoded path transversal detected: " << fullpath << "(decoded: " << decoded << ")" << std::endl;
		return false;
	}

	//normalization uhmmmm
	std::string normalized = normalizePath(decoded);

	//parsing path
	std::string path, query;
	parsePath(normalized, path, query);

	//filling structure
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
            
            // Convertir hex a número
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

//NEW FUNCTIOOOON
std::string RequestLine::normalizePath(const std::string &path)
{
	std::vector <std::string> parts;
	std::istringstream iss(path);
	std::string part;

	//Divide in / find a / and divide
	while(std::getline(iss, part, '/')) {
		if(part.empty() || part == ".")
			continue;
		if(part == "..") {
			//have parts
			if(!parts.empty())
				parts.pop_back();
			//dont have parts and gonna ignore it
		}
		else
			parts.push_back(part);
	}

	//redo part
	std::string result = "/";
	for(size_t i = 0; i<parts.size(); i++) {
		if(i>0)
			result += "/";
		result += parts[i];
	}

	return result;
}