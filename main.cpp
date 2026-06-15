#include "CGIPipes.hpp"
#include "ClientManager.hpp"
#include "Server.hpp"
#include "EventLoop.hpp"
#include "LocationParsing.hpp"
#include "BodyBuilder.hpp"
#include "ParseConfig.hpp"
#include "StartServer.hpp"
#include "Config.hpp"
#include "Colors.hpp"
#include <iostream>
#include <unistd.h>

volatile sig_atomic_t g_shutdown = 0;

void printRequest(const HttpRequest &req)
{
    std::cout << "\n" << B_YELLOW << "──── REQUEST ────────────────────────────" << RESET << std::endl;
    std::cout << "  " << B_WHITE << "Method: " << RESET << req.method << std::endl;
    std::cout << "  " << B_WHITE << "Path:   " << RESET << req.path << std::endl;
    if (!req.queryString.empty())
        std::cout << "  " << B_WHITE << "Query:  " << RESET << req.queryString << std::endl;
    std::cout << "  " << B_WHITE << "HTTP:   " << RESET << req.httpVersion << std::endl;
    std::cout << "  " << B_WHITE << "IP:     " << RESET << req.clientIP << std::endl;

    if (!req.cookies.empty()) {
        std::cout << "  " << B_WHITE << "Cookies (" << req.cookies.size() << "):" << RESET << std::endl;
        for (std::map<std::string, std::string>::const_iterator it = req.cookies.begin(); it != req.cookies.end(); it++)
            std::cout << "    " << CYAN << it->first << RESET << " = " << it->second << std::endl;
    }
}

void printHeaders(const HttpRequest &request)
{
    std::cout << "\n" << B_BLUE << "──── HEADERS (" << request.headers.size() << ") ──────────────────────" << RESET << std::endl;
    for (std::map<std::string, std::string>::const_iterator it = request.headers.begin(); it != request.headers.end(); ++it)
        std::cout << "  " << CYAN << it->first << RESET << ": " << it->second << std::endl;
}

int main(int ac, char **ag)
{
    if (ac != 2)
    {
        std::cerr << B_RED << "[ERROR]" << RESET << " Usage: " << ag[0] << " <config_file>" << std::endl;
        return (1);
    }
    try
    {
        std::cout << B_CYAN << "\n  WEBSERV" << RESET << "  starting..." << std::endl;
        std::cout << B_CYAN << "  config: " << RESET << ag[1] << "\n" << std::endl;

        ParseConfig configParser;
        configParser.start(ag[1]);
        std::vector<ServerConfig> allServersConfig = configParser.getAllServerConfig();

        // ── CONFIG DISPLAY ──────────────────────────────────────
        std::cout << B_BLUE << "┌─ CONFIGURATION ";
        std::cout << std::string(42, '-') << "┐" << RESET << std::endl;
        std::cout << B_WHITE << "│" << RESET << " Servers: " << allServersConfig.size() << std::endl;

        for (size_t i = 0; i < allServersConfig.size(); i++)
        {
            ServerConfig &s = allServersConfig[i];
            std::cout << B_BLUE << "│" << RESET << std::endl;
            std::cout << B_BLUE << "│" << RESET << " " << MAGENTA << "Server[" << i << "]" << RESET << std::endl;
            std::cout << B_BLUE << "│" << RESET << "   Port:     " << B_CYAN << s.port << RESET << std::endl;
            std::cout << B_BLUE << "│" << RESET << "   Host:     " << s.host << std::endl;
            std::cout << B_BLUE << "│" << RESET << "   Name:     " << s.serverName << std::endl;
            std::cout << B_BLUE << "│" << RESET << "   Root:     " << s.root << std::endl;
            std::cout << B_BLUE << "│" << RESET << "   MaxBody:  " << s.clientMaxBodySize << " bytes" << std::endl;
            std::cout << B_BLUE << "│" << RESET << "   Index:    " << (s.index.empty() ? "-" : s.index) << std::endl;

            for (size_t j = 0; j < s.locations.size(); j++)
            {
                LocationConfig &loc = s.locations[j];
                std::cout << B_BLUE << "│" << RESET << std::endl;
                std::cout << B_BLUE << "│" << RESET << "   " << MAGENTA << "Location[" << j << "]  " << loc.uri << RESET << std::endl;
                if (!loc.rootLocation.empty())
                    std::cout << B_BLUE << "│" << RESET << "     Root:      " << loc.rootLocation << std::endl;
                if (!loc.allow_methods.empty())
                {
                    std::cout << B_BLUE << "│" << RESET << "     Methods:   ";
                    for (size_t m = 0; m < loc.allow_methods.size(); m++)
                        std::cout << B_CYAN << loc.allow_methods[m] << RESET << " ";
                    std::cout << std::endl;
                }
                if (!loc.uploadPath.empty())
                    std::cout << B_BLUE << "│" << RESET << "     Upload:    " << loc.uploadPath << std::endl;
                if (!loc.index.empty())
                    std::cout << B_BLUE << "│" << RESET << "     Index:     " << loc.index[0] << std::endl;
                std::cout << B_BLUE << "│" << RESET << "     Autoindex: " << (loc.autoindex ? "on" : "off") << std::endl;
            }
        }
        std::cout << B_BLUE << "└" << std::string(58, '-') << "┘" << RESET << "\n" << std::endl;

        std::cout << B_GREEN << "[OK]" << RESET << " Config parsed successfully" << std::endl;
        std::cout << B_CYAN << "[SERVER]" << RESET << " Starting...\n" << std::endl;

        StartServer starter;
        if (starter.start(allServersConfig))
            return (1);

        std::cout << "\n" << B_YELLOW << "[SERVER]" << RESET << " Stopped gracefully" << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cerr << B_RED << "[ERROR]" << RESET << " " << e.what() << std::endl;
        return (1);
    }
    return (0);
}