/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpResponse.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vali <vali@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/01 17:29:28 by dianarituay       #+#    #+#             */
/*   Updated: 2026/03/08 19:57:53 by vali             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPRESPONSE_HPP
#define HTTPRESPONSE_HPP

#include <map>
#include <string>
#include <vector>
#include "CGIEnvironment.hpp"

struct HttpResponse
{
	int statusCode;

	std::map<std::string, std::string> headers; //headers builder
	std::vector<std::string> setCookieHeaders;
	std::string body; 
	CGIProcess cgiProcess;
	
	HttpResponse() :
		statusCode(200), //default 200 code OK
		body("")
	{}
};

#endif