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