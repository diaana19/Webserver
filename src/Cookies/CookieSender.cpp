#include "CookieSender.hpp"
#include <sstream>
#include <iostream>

CookieSender::CookieSender()
{

}

CookieSender::~CookieSender()
{

}

std::string CookieSender::buildSetCookie(const std::string& name, const std::string& value, int maxAge, const std::string& path)
{
    if (name.empty())
    {
        std::cerr << "[CookieSender] Cookie name cannot be empty" << std::endl;
        return "";
    }
    std::ostringstream cookie; 
    cookie << "Set-Cookie: " << name << "=" << value;

    if (maxAge > 0)
        cookie << "; Max-Age=" << maxAge;
    if (!path.empty())
        cookie << "; Path=" << path;
    
    cookie << "; HttpOnly; SameSite=Lax";
    cookie << "\r\n"; 
    std::cout << "[CookieSender] Built: " << cookie.str();
    return (cookie.str()); 
}