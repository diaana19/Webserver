/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HeadersParsing.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dirituay <dirituay@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/30 10:52:16 by dirituay          #+#    #+#             */
/*   Updated: 2026/01/30 13:27:03 by dirituay         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

/* 
STRUCT HTTP REQUEST LLENA
=> Request Line
request.method = "POST"
request.path = "/api/login"
request.queryString = "redirect=home"
request.httpVersion = "HTTP/1.1"

=> Headers
request.headers["Host"] = "localhost:8080"
request.headers["User-Agent"] = "curl/7.68.0"
request.headers["Accept"] = "application/json"
request.headers["Content-Type"] = "application/json"
request.headers["Content-Length"] = "52"

=> Body
request.body = "{\"username\":\"diana\",\"password\":\"mypassword123456\"}"
parse => rawRequest: el http request completo como texto
=> request: la struct q se llena
*/