#include "FileServer.hpp"
#include <sstream>
#include <ctime>
#include <cerrno>
#include <cstring>

FileServer :: FileServer()
{}

FileServer :: ~FileServer()
{}

bool FileServer :: fileExist(const std::string &path)
{
    struct stat buffer;
    return(stat(path.c_str(), &buffer) == 0);
}

bool FileServer:: isDirectory(const std::string &path)
{
    struct  stat buffer;

    if(stat(path.c_str(), &buffer) != 0)
        return false;
    return S_ISDIR(buffer.st_mode);
}

bool FileServer :: hasPermission(const std::string &path)
{
    return(access(path.c_str(), R_OK) == 0);
}

size_t FileServer :: getFileSize(const std::string &path)
{
    struct stat buffer;

    if(stat(path.c_str(), &buffer) != 0)
    {
        std::cerr << "[FileServer] Error getting file size : " << strerror(errno) << std::endl;
        return 0;
    }
    return static_cast<size_t>(buffer.st_size);
}

std::string FileServer :: getLastModified(const std::string &path)
{
    struct stat buffer;

    if(stat(path.c_str(), &buffer) != 0)
        return "";

    
    char timeStr[100];
    struct tm* timeinfo = gmtime(&buffer.st_mtime);
    strftime(timeStr, sizeof(timeStr), "%a, %d %b %Y %H:%M:%S GMT", timeinfo);
    return std::string(timeStr);
}

std::string FileServer :: readTextFile(const std::string &path)
{
    std::ifstream file(path.c_str(), std::ios::in);

    if(!file.is_open())
    {
        std::cerr << "[FileServer] Cannot open text file: " << path << std::endl;
        return "";
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    file.close();

    return buffer.str();
}

std::string FileServer :: readBinaryFile(const std::string &path)
{
    std::ifstream file(path.c_str(), std::ios::in | std::ios::binary);
    if(!file.is_open())
    {
        std::cerr << "[FileServer] Cannot open binary file: " << path << std::endl;
        return ""; 
    }

    
    std::stringstream buffer;
    buffer << file.rdbuf();
    file.close();

    return buffer.str();
}

std::string FileServer :: readFile(const std::string &path)
{
    
    if(!fileExist(path))
    {
        std::cerr << "[FileServer] File not found : " << path << std::endl;
        return "";
    }

    
    if(!hasPermission(path))
    {
        std::cerr << "[FileServer] No read permission: " << path << std::endl;
        return "";
    }

    
    if(isDirectory(path))
    {
        std::cerr << "[FileServer] Path is a directory : " << path << std::endl;
        return ""; 
    }

    
    std::string extension;
    size_t dotPos = path.find_last_of('.');
    if(dotPos != std::string::npos)
        extension = path.substr(dotPos);

    
    if(extension == ".png" || extension == ".jpg" || extension == ".jpeg" ||
       extension == ".gif" || extension == ".ico" || extension == ".pdf" ||
       extension == ".zip" || extension == ".tar" || extension == ".gz")
       {
            std::cout << "[FileServer] Reading binary file: " << path << std::endl;
            return readBinaryFile(path);
       }

       
       std::cout << "[Fileserver] Reading text file: " << path << std::endl;
       return readTextFile(path);
}