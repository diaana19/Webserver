/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CookieParser.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dianarituay <dianarituay@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/19 13:39:29 by dianarituay       #+#    #+#             */
/*   Updated: 2026/03/19 13:39:30 by dianarituay      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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