/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Colors.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dirituay <dirituay@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/23 12:46:10 by dirituay          #+#    #+#             */
/*   Updated: 2026/03/23 12:46:12 by dirituay         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <iostream>
#include <string>

// ANSI Colors
#define RESET   "\033[0m"
#define BOLD    "\033[1m"
#define DIM     "\033[2m"

#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"
#define WHITE   "\033[37m"

#define B_RED     "\033[1;31m"
#define B_GREEN   "\033[1;32m"
#define B_YELLOW  "\033[1;33m"
#define B_BLUE    "\033[1;34m"
#define B_MAGENTA "\033[1;35m"
#define B_CYAN    "\033[1;36m"
#define B_WHITE   "\033[1;37m"

class Logger {
public:
    static void server(const std::string &msg) {
        std::cout << B_CYAN << "[SERVER]  " << RESET << msg << std::endl;
    }
    static void client(const std::string &msg) {
        std::cout << B_GREEN << "[CLIENT]  " << RESET << msg << std::endl;
    }
    static void disconnect(const std::string &msg) {
        std::cout << B_RED << "[CLIENT]  " << RESET << msg << std::endl;
    }
    static void request(const std::string &method, const std::string &path) {
        std::cout << B_YELLOW << "[" << method << "]" << RESET
                  << std::string(9 - method.size(), ' ')
                  << path << std::endl;
    }
    static void file(const std::string &msg) {
        std::cout << DIM << "[FILE]    " << RESET << msg << std::endl;
    }
    static void cgi(const std::string &msg) {
        std::cout << B_MAGENTA << "[CGI]     " << RESET << msg << std::endl;
    }
    static void error(const std::string &msg) {
        std::cerr << B_RED << "[ERROR]   " << RESET << msg << std::endl;
    }
    static void config(const std::string &msg) {
        std::cout << B_BLUE << msg << RESET << std::endl;
    }
    static void send(const std::string &msg) {
        std::cout << DIM << "[SEND]    " << RESET << msg << std::endl;
    }
};