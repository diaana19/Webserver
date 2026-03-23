/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GetHandler.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dianarituay <dianarituay@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/26 10:42:01 by dianarituay       #+#    #+#             */
/*   Updated: 2026/03/01 19:21:52 by dianarituay      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "HttpRequest.hpp"
#include "HttpResponse.hpp"
#include "Config.hpp"
#include "BodyBuilder.hpp"
#include "Colors.hpp"
#include <string>
#include <vector>

class GetHandler
{
    private:
        BodyBuilder _bodyBuilder;
        bool fileExist(const std::string &path);
        bool isDirectory(const std::string &path);
        bool hasReadPermission(const std::string &path);
        std::string buildFilePath(const std::string &root, const std::string &uri);
        std::string findIndexFile(const std::string &dirPath, const std::vector <std::string> &indexNames);
        std::string generateDirectoryListing(const std::string &dirPath, const std::string &uri);
        std::vector<std::string> listDirectory(const std::string &dirPath);
        
    public:
        GetHandler();
        ~GetHandler();
    
        HttpResponse handle(const HttpRequest &req, const LocationConfig& loc, const ServerConfig& server);
};
