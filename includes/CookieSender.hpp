#ifndef COOKIESENDER_HPP
#define COOKIESENDER_HPP

#include <string>

class CookieSender
{
    private: 

    public:
        CookieSender();
         ~CookieSender();
         std::string buildSetCookie(const std::string& name, const std::string& value, int maxAge, const std::string& path);
};

#endif
