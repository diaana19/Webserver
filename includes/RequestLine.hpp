/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestLine.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vali <vali@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/01 17:28:42 by dianarituay       #+#    #+#             */
/*   Updated: 2026/03/19 18:53:18 by vali             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <string>
#include "HttpRequest.hpp"

class RequestLine
{
    private:
        std::string trim(const std::string &str);
        void parsePath(const std::string &fullPath, std::string &path, std::string &query);
        std::string urlDecode(const std::string &str);
    public:
        RequestLine();
        ~RequestLine();
        std::string normalizePath(const std::string &path);
        bool parse(const std::string &line, HttpRequest &req);
};