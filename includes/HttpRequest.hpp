/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vali <vali@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/28 10:21:57 by dianarituay       #+#    #+#             */
/*   Updated: 2026/03/10 23:21:40 by vali             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPREQUEST_HPP
# define HTTPREQUEST_HPP

# include <map>
# include <vector>
# include <string>
# include "File.hpp"
# include "Config.hpp"

struct	HttpRequest
{
    std::string method;
    std::string path;
    std::string queryString;
    std::string httpVersion;

	std::map<std::string, std::string> headers; // header parser
	std::vector<File> allFiles;
	ServerConfig serverConfig;
	LocationConfig location;
	std::string body;
	std::string clientIP;

	//New cookies
	std::map <std::string, std::string> cookies;

	HttpRequest() :
	method(""),
	path(""),
	queryString(""),
	httpVersion(""),
	body(""),
	clientIP("127.0.0.1")
	{}
};

#endif