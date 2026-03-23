/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerParsing.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dianarituay <dianarituay@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/20 13:29:13 by dirituay          #+#    #+#             */
/*   Updated: 2026/01/27 21:32:20 by dianarituay      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVERPARSING_HPP
#define SERVERPARSING_HPP

#include "Config.hpp"
#include "Token.hpp"
#include "LocationParsing.hpp"
#include <vector>
#include <string>
#include <stdexcept>

class ServerParsing
{
	private:
			std::string intToString(int n);
			int parsePort(const std::vector<Token>& tokens, size_t& pos);
			std::string parseHost( const std::vector<Token>& tokens, size_t& pos);
			std::string parseRoot(const std::vector<Token>& tokens, size_t& pos);
			std::string parseServerName(const std::vector<Token>& tokens, size_t& pos);
			size_t parseClientMaxBodySize(const std::vector<Token>& tokens, size_t& pos);
			std::string parseIndex(const std::vector<Token> &tokens, size_t &pos);
			void parseErrorPage(const std::vector<Token>& tokens, size_t& pos, ServerConfig& config);

			//funciones aux.
			void expectToken(const std::vector<Token>& tokens, size_t& pos, TokenType expected);
			void skipSemicolon(const std::vector<Token>& tokens, size_t& pos);
	public:
		ServerParsing();
		~ServerParsing();
		ServerConfig parseServer(const std::vector<Token>& tokens, size_t& pos); //referencia pq no copia, modifica
};

#endif
