/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GetHandler.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vali <vali@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/17 13:46:04 by dianarituay       #+#    #+#             */
/*   Updated: 2026/03/08 21:57:49 by vali             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "GetHandler.hpp"
#include "PostHandler.hpp"
#include "StatusBuilder.hpp"
#include "DirectoryLister.hpp"
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <iostream>
#include <sstream>

GetHandler :: GetHandler()
{}
GetHandler :: ~GetHandler()
{}

bool GetHandler :: fileExist(const std::string &path)
{
    struct stat buffer;
    return (stat(path.c_str(), &buffer) == 0);
}
bool GetHandler :: isDirectory(const std::string &path)
{
    struct stat buffer;
    if(stat(path.c_str(), &buffer) != 0)
        return false;
    return S_ISDIR(buffer.st_mode);
}
bool GetHandler :: hasReadPermission(const std::string &path)
{
    return (access(path.c_str(), R_OK) == 0);
}
std::string GetHandler :: buildFilePath(const std::string &root, const std::string &uri)
{
    std::string path = root;

    //be sure the root finish with /
    if(!path.empty() && path[path.length() - 1] != '/')
        path += "/";

    // delete / start of uri if exiwst
    std::string cleanUri = uri;
    if(!cleanUri.empty() && cleanUri[0] == '/')
        cleanUri = cleanUri.substr(1);

    return path + cleanUri;
}

std::string GetHandler :: findIndexFile(const std::string &dirPath, const std::vector<std::string> &indexNames)
{
    std::string basePath = dirPath;
    
    if(!basePath.empty() && basePath[basePath.length() - 1] != '/')
        basePath += "/";
    
    // Probar cada index en orden
    for(size_t i = 0; i < indexNames.size(); i++)
    {
        std::string indexPath = basePath + indexNames[i];
        
        if(fileExist(indexPath))
        {
            std::cout << B_CYAN << "[GET] Found index file: " << RESET << indexPath << std::endl;
            return indexPath;
        }
    }
    
    // Ningún index encontrado
    return "";
}

std::vector<std::string> GetHandler :: listDirectory(const std::string &dirPath)
{
    std::vector<std::string> files;
    DIR* dir = opendir(dirPath.c_str());

    if(!dir)
        return files;
    struct dirent*entry;
    while((entry = readdir(dir)) != NULL)
    {
        std::string name = entry->d_name;
    
        //ignore . and ,,
        if(name == "." || name == "..")
            continue;
        files.push_back(name);
    }
    closedir(dir);
    return files;
}

std::string GetHandler :: generateDirectoryListing(const std::string &dirPath, const std::string &uri)
{
    std::vector<std::string> files = listDirectory(dirPath);
    
    std::ostringstream html;
    
    html << "<!DOCTYPE html>\n";
    html << "<html>\n";
    html << "<head>\n";
    html << "    <meta charset=\"UTF-8\">\n";
    html << "    <title>Index of " << uri << "</title>\n";
    html << "    <style>\n";
    html << "        body { font-family: Arial, sans-serif; margin: 40px; }\n";
    html << "        h1 { color: #333; }\n";
    html << "        ul { list-style: none; padding: 0; }\n";
    html << "        li { padding: 8px; border-bottom: 1px solid #eee; }\n";
    html << "        a { text-decoration: none; color: #0066cc; }\n";
    html << "        a:hover { text-decoration: underline; }\n";
    html << "        .dir { font-weight: bold; }\n";
    html << "        .file { color: #666; }\n";
    html << "    </style>\n";
    html << "</head>\n";
    html << "<body>\n";
    html << "    <h1>Index of " << uri << "</h1>\n";
    html << "    <ul>\n";
    
    // Link al directorio padre
    if(uri != "/")
    {
        html << "        <li><a href=\"..\" class=\"dir\">📁 ../</a></li>\n";
    }
    
    // Listar archivos y directorios
    for(size_t i = 0; i < files.size(); i++)
    {
        std::string fullPath = dirPath;
        if(!fullPath.empty() && fullPath[fullPath.length() - 1] != '/')
            fullPath += "/";
        fullPath += files[i];
        
        std::string href = uri;
        if(!href.empty() && href[href.length() - 1] != '/')
            href += "/";
        href += files[i];
        
        if(isDirectory(fullPath))
        {
            html << "        <li><a href=\"" << href << "/\" class=\"dir\">📁 " 
                 << files[i] << "/</a></li>\n";
        }
        else
        {
            html << "        <li><a href=\"" << href << "\" class=\"file\">📄 " 
                 << files[i] << "</a></li>\n";
        }
    }
    
    html << "    </ul>\n";
    html << "    <hr>\n";
    html << "    <p><em>webserv/1.0</em></p>\n";
    html << "</body>\n";
    html << "</html>\n";
    
    return html.str();
}
HttpResponse GetHandler :: handle(const HttpRequest &req, const LocationConfig& loc, const ServerConfig& server)
{
    HttpResponse response;
    if (!loc.allow_methods.empty())
    {
        bool allowed = false;
        for (size_t i = 0; i < loc.allow_methods.size(); i++)
        {
            if (loc.allow_methods[i] == req.method)
            {
                allowed = true;
                break;
            }
        }
        if (!allowed)
        {
            response.statusCode = 405;
            return (response);
        }
    }
    if (loc.cgi_On)
    {
        PostHandler postHandler;
        return postHandler.handleCGI(req, loc, server);
    }
    std::cout << B_CYAN << "[GET] Handling: " << RESET << req.path << std::endl;

    // 1. Construir path completo del archivo
    std::string filePath = buildFilePath(loc.rootLocation, req.path);
    
    std::cout << B_CYAN << "[GET] Looking for file: " << RESET << filePath << std::endl;
    
    // 2. Verificar si existe
    if(!fileExist(filePath))
    {
        std::cout << RED << "[GET] File not found: " << RESET << filePath << std::endl;
        response.statusCode = 404;
        response.headers["Content-Type"] = "text/html";
        // response.body = "<html><body><h1>404 Not Found</h1></body></html>";
        return response;
    }
    
    // 3. Verificar permisos de lectura
    if(!hasReadPermission(filePath))
    {
        std::cout << RED << "[GET] Permission denied: " << RESET << filePath << std::endl;
        response.statusCode = 403;
        response.headers["Content-Type"] = "text/html";
        response.body = "<html><body><h1>403 Forbidden</h1></body></html>";
        return response;
    }
    
    // 4. Si es un directorio
    if(isDirectory(filePath))
    {
        std::cout << B_CYAN << "[GET] Path is a directory" << RESET << std::endl;
        
        // 4.1 Buscar archivo index
        std::string indexPath = findIndexFile(filePath, loc.index);
        
        if(!indexPath.empty())
        {
            std::cout << B_CYAN << "[GET] Found index file: " << RESET << indexPath << std::endl;
            
            response.statusCode = 200;
            response.headers["Content-Type"] = _bodyBuilder.getMimeType(indexPath);
            response.body = _bodyBuilder.readFile(indexPath);
            return response;
        }
        
        // 4.2 Si autoindex está activado, generar listado
        if(loc.autoindex != 0)
        {
            std::cout << B_CYAN << "[GET] Generating directory listing" << RESET << std::endl;
            
            DirectoryLister lister;
            response.statusCode = 200;
            response.headers["Content-Type"] = "text/html";
            response.body = lister.generateHTML(filePath, req.path);
            return (response);
        }
        
        // 4.3 Si no hay index ni autoindex, retornar 403
        std::cout << RED << "[GET] No index and autoindex off" << RESET << std::endl;
        response.statusCode = 403;
        response.headers["Content-Type"] = "text/html";
        response.body = "<html><body><h1>403 Forbidden</h1></body></html>";
        return response;
    }
    
    // 5. Es un archivo normal, servirlo
    std::cout << B_CYAN << "[GET] Serving file: " << RESET << filePath << std::endl;
    
    response.statusCode = 200;
    response.headers["Content-Type"] = _bodyBuilder.getMimeType(filePath);
    response.body = _bodyBuilder.readFile(filePath);
    
    if(response.body.empty())
    {
        std::cout << RED << "[GET] Error reading file" << RESET << std::endl;
        response.statusCode = 500;
        response.headers["Content-Type"] = "text/html";
        response.body = "<html><body><h1>500 Internal Server Error</h1></body></html>";
    }
    
    return response;
}