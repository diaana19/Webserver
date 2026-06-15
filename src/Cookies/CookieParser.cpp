#include "CookieParser.hpp"
#include <sstream>
#include <cctype>

CookieParser :: CookieParser() {}
CookieParser :: ~CookieParser() {}

std::string CookieParser::trim(const std::string &str)
{
    
    size_t start = 0;
    size_t end = str.length();

    while(start < end && std::isspace(str[start]))
        start++;

    while(end > start && std::isspace(str[end-1]))
        end--;
    return str.substr(start, end - start);
}

std::pair <std::string, std::string> CookieParser :: parseCookiePair(const std::string &pair)
{
    size_t equalPos = pair.find('=');
    
    if(equalPos == std::string::npos)
    {
        
        std::cerr << RED << "[CookieParser] Invalid cookie pair: " << RESET << pair << std::endl;
        return std::make_pair("", "");
    }

    std::string name = trim(pair.substr(0, equalPos));
    std::string value = trim(pair.substr(equalPos + 1));

    return std::make_pair(name, value);
}

std::map <std::string, std::string> CookieParser :: parse(const std::string &cookieHeader)
{
    std::map <std::string, std::string> cookies;

    if(cookieHeader.empty()) {
        std::cout << RED << "[CookieParser] Empty cookie header " << RESET << std::endl;
        return cookies;
    }

    std::cout << B_CYAN << "[CookieParser] Parsing: " << RESET << cookieHeader << std::endl;

    
    std::istringstream stream(cookieHeader);
    std::string pair;

    while(std::getline(stream, pair, ';')) {
        
        std::pair <std::string, std::string> cookie = parseCookiePair(pair);

        if(!cookie.first.empty()) {
            cookies[cookie.first] = cookie.second;
        }
    }

    std::cout << B_CYAN << "[CookieParser] Total cookies:  " << RESET << cookies.size() << std::endl;
    return cookies;
}

std::string CookieParser :: getCookie (const std::map<std::string, std::string> &cookies, const std::string &name)
{
    std::map <std::string, std::string> :: const_iterator it = cookies.find(name);
    
    if(it != cookies.end())
        return it->second;

    return "";
}