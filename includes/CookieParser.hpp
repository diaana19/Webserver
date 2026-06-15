#pragma once
#include <string>
#include <map>
#include <iostream>
#include "Colors.hpp"

class CookieParser
{
    private:
        std::string trim(const std::string &str);
        std::pair<std::string, std::string> parseCookiePair(const std::string &pair);

    public:
        CookieParser();
        ~CookieParser();
        std::map <std::string, std::string> parse(const std::string &cookieHeader);
        std::string getCookie(const std::map <std::string, std::string> &cookies, const std::string &name);
};