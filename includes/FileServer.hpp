#pragma once
#include <string>
#include <sys/stat.h>
#include <unistd.h>
#include <fstream>
#include <iostream>

class FileServer
{
    private:
        std::string readTextFile(const std::string &path);
        std::string readBinaryFile(const std::string &path);
    public:
        FileServer();
        ~FileServer();
    
        std::string readFile(const std::string &path);
        bool fileExist(const std::string &path);
        bool isDirectory(const std::string &path);
        size_t getFileSize(const std::string &path);
        bool hasPermission(const std::string &path);
        std::string getLastModified(const std::string &path);
};