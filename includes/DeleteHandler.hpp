/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DeleteHandler.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dianarituay <dianarituay@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/14 15:58:37 by dianarituay       #+#    #+#             */
/*   Updated: 2026/03/01 19:22:52 by dianarituay      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
# ifndef DELETEHANDLER_HPP
# define DELETEHANDLER_HPP

# include <iostream>
# include <string>
# include "Config.hpp"
# include "HttpRequest.hpp"
# include "HttpResponse.hpp"
# include "RedirectHandler.hpp"
// # include "StatusBuilder.hpp"

# include <fstream>
# include <sstream>
# include <cstdlib>
# include <cstdio>
#include <sys/stat.h>
#include <unistd.h>

class DeleteHandler
{
	private:
		bool verifyMethod(const std::string &method, const LocationConfig &location);
		bool doIsItExist(const std::string &pathfile);
		bool isRemoved(const std::string &pathfile);
		bool canDelete(const std::string &pathfile);
		std::string combinePath(const std::string &root, const std::string &uri);

		
	public:
		DeleteHandler();
		~DeleteHandler();
		
		HttpResponse handle(const HttpRequest &httpReq, const LocationConfig &location, const ServerConfig& server);
};

#endif