/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CookieSender.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dianarituay <dianarituay@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/19 13:38:43 by dianarituay       #+#    #+#             */
/*   Updated: 2026/03/19 13:38:45 by dianarituay      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
    std::ostringstream cookie; //escribe en una string
    cookie << "Set-Cookie: " << name << "=" << value;

    if (maxAge > 0)
        cookie << "; Max-Age=" << maxAge;
    if (!path.empty())
        cookie << "; Path=" << path;
    //httponly el JS del browser no puede leer esta cookie, protege contra ataques xss(robo de sesion via js)
    cookie << "; HttpOnly; SameSite=Lax";
    cookie << "\r\n"; // el header http siempre termina asi
    std::cout << "[CookieSender] Built: " << cookie.str();
    return (cookie.str()); 
}