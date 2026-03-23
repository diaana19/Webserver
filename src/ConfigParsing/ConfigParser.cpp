/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigParser.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dianarituay <dianarituay@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/25 20:39:27 by dianarituay       #+#    #+#             */
/*   Updated: 2026/01/25 20:39:29 by dianarituay      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"
#include "ServerParsing.hpp"
#include "Token.hpp"

Config ConfigParser::parse (const std::vector<Token> &tokens)
{
    Config config;
    size_t i = 0;

    while(i < tokens.size())
    {
        if (tokens[i].type == TOKEN_SERVER) {
            ServerParsing serverParser;
            ServerConfig server = serverParser.parseServer(tokens, i);
            config.servers.push_back(server);
        }
        else if (tokens[i].type == TOKEN_EOF)
            break;
        else
            i++;
    }
    return config;
}